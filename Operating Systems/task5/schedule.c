/* This file contains the scheduling policy for SCHED
 *
 * The entry points are:
 *   do_noquantum:        Called on behalf of process' that run out of quantum
 *   do_start_scheduling  Request to start scheduling a proc
 *   do_stop_scheduling   Request to stop scheduling a proc
 *   do_nice		  Request to change the nice level on a proc
 *   init_scheduling      Called from main.c to set up/prepare scheduling
 */
#include "sched.h"
#include "schedproc.h"
#include <assert.h>
#include <minix/com.h>
#include <machine/archtypes.h>
#include "kernel/proc.h" /* for queue constants */

/************************** SO-TASK-5 ***************************************/
#include <stdio.h>
#include <unistd.h>
/****************************************************************************/

static minix_timer_t sched_timer;
static unsigned balance_timeout;

#define BALANCE_TIMEOUT	5 /* how often to balance queues in seconds */


/************************** SO-TASK-5 ***************************************/

#define MAX_TOKENS 8
#define SCHED_FACTOR 0.6

static clock_t last_refill_time; /* previous time of adding new tokens;
                        used to check how many new tokens should be introduced */
static int last_process; /* last process that got new tokens from tokens_count
                    during the call to distribute_tokens (variable used to keep
                    a loop order of allocating new tokens*/

static void refill_tokens(); /* refill global tokens_count; used in
 *                                                              balance_queues */
static void distribute_tokens(clock_t new_tokens); /* from global_tokens to
				processes' pools; used in do_noquantum() and balance_queues() */
static void decrease_tokens(const int proc_nr); /* decrease number of process'
				  tokens by the amount of time it used; used in do_noquantum() */
static clock_t process_systime_difference(const int proc_nr); /* returns time
				used by the process since last checking, updates the old_systime
				field in schedproc struct by current systime used by process */
static clock_t global_systime_difference(); /* returns time difference from the
				last checking of global variable last_refill_time; updates
				last_refill time with the current uptime */

/****************************************************************************/


static int schedule_process(struct schedproc * rmp, unsigned flags);
static void balance_queues(minix_timer_t *tp);

#define SCHEDULE_CHANGE_PRIO	0x1
#define SCHEDULE_CHANGE_QUANTUM	0x2
#define SCHEDULE_CHANGE_CPU	0x4

#define SCHEDULE_CHANGE_ALL	(	\
		SCHEDULE_CHANGE_PRIO	|	\
		SCHEDULE_CHANGE_QUANTUM	|	\
		SCHEDULE_CHANGE_CPU		\
		)

#define schedule_process_local(p)	\
	schedule_process(p, SCHEDULE_CHANGE_PRIO | SCHEDULE_CHANGE_QUANTUM)
#define schedule_process_migrate(p)	\
	schedule_process(p, SCHEDULE_CHANGE_CPU)

#define CPU_DEAD	-1

#define cpu_is_available(c)	(cpu_proc[c] >= 0)

#define DEFAULT_USER_TIME_SLICE 200

/* processes created by RS are sysytem processes */
#define is_system_proc(p)	((p)->parent == RS_PROC_NR)

static unsigned cpu_proc[CONFIG_MAX_CPUS];



static void pick_cpu(struct schedproc * proc)
{
#ifdef CONFIG_SMP
    unsigned cpu, c;
	unsigned cpu_load = (unsigned) -1;

	if (machine.processors_count == 1) {
		proc->cpu = machine.bsp_id;
		return;
	}

	/* schedule sysytem processes only on the boot cpu */
	if (is_system_proc(proc)) {
		proc->cpu = machine.bsp_id;
		return;
	}

	/* if no other cpu available, try BSP */
	cpu = machine.bsp_id;
	for (c = 0; c < machine.processors_count; c++) {
		/* skip dead cpus */
		if (!cpu_is_available(c))
			continue;
		if (c != machine.bsp_id && cpu_load > cpu_proc[c]) {
			cpu_load = cpu_proc[c];
			cpu = c;
		}
	}
	proc->cpu = cpu;
	cpu_proc[cpu]++;
#else
    proc->cpu = 0;
#endif
}

/*********************************** SO-TASK-5 *******************************/

/* Pass new_tokens to processes from schedproc, starting from last_process + 1 (mod NR_PROC)*/
void distribute_tokens(clock_t new_tokens)
{
    register struct schedproc *rmp;
    int proc_nr;

    for (proc_nr=(last_process + 1) % NR_PROCS, rmp=&schedproc[proc_nr];
         new_tokens > 0 && proc_nr != last_process;
         proc_nr = (proc_nr + 1) % NR_PROCS, rmp = &schedproc[proc_nr]) {

        if (!(rmp->flags & IN_USE)) continue;

        int previous_tokens_sign = rmp->tokens_sign;

        if(rmp->tokens_sign > 0) {
            rmp->tokens += new_tokens; /* add all tokens to rmp */
        }
        else { /* negative number of tokens */
            if (new_tokens > rmp->tokens) { /* sign will change */
                rmp->tokens = new_tokens - rmp->tokens; /* add only what's left in
                        new_tokens after neutralizing negative number of rmp->tokens */
                rmp->tokens_sign = POSITIVE;
            }
            else { /* sign won't change */
                rmp->tokens -= new_tokens; /* just reduce negativity */
            }
        }
        if(rmp->tokens > MAX_TOKENS) {
            new_tokens = rmp->tokens - MAX_TOKENS;
            rmp->tokens = MAX_TOKENS;
        }

//        if (previous_tokens_sign != rmp->tokens_sign) {
//            /* process was blocked from scheduling because of negative number of tokens
//             * but it now has positive number so it should be manually scheduled */
//            schedule_process_local(rmp);
//        }

    }
    /* undo increasing last_process at the end of for loop */
    last_process = proc_nr == 0 ? NR_PROCS - 1 : proc_nr - 1;
}

/* Take tokens from process' tokens pool on the basis of the time it used */
void decrease_tokens(const int proc_nr)
{
    register struct schedproc *rmp;
    rmp = &schedproc[proc_nr];
    clock_t difference = process_systime_difference(proc_nr);
    if (rmp->tokens_sign > 0) {
        if (rmp->tokens > difference) {
            rmp->tokens -= difference;
        }
        else { /* difference > rmp->tokens */
            rmp->tokens = difference - rmp->tokens;
            rmp->tokens_sign = NEGATIVE;
        }
    } else { /* negative number of tokens */
        rmp->tokens += difference;
    }
}

/* Returns the system time used by the process since the last check. Updates
the old_systime field in process' index of schedproc table by the current systime */
clock_t process_systime_difference(const int proc_nr)
{
    register struct schedproc *rmp;
    rmp = &schedproc[proc_nr];

    clock_t sys_time;
    sys_times(rmp->endpoint, NULL, &sys_time, NULL, NULL);

    clock_t difference = sys_time - rmp->old_systime;
    rmp->old_systime = sys_time;

    return difference;

}

/* Returns system time difference since the last check of global variable
last_refill_time. Updates last_refill_time with the current system time. */
clock_t global_systime_difference()
{
    clock_t up_time;
    sys_times(NONE, NULL, NULL, &up_time, NULL);

    clock_t previous = last_refill_time;
    last_refill_time = up_time;

    return up_time - previous;
}
/*****************************************************************************/


/*===========================================================================*
 *				do_noquantum				     *
 *===========================================================================*/

int do_noquantum(message *m_ptr)
{
    register struct schedproc *rmp;
    int rv, proc_nr_n;

    if (sched_isokendpt(m_ptr->m_source, &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OOQ msg %u.\n",
               m_ptr->m_source);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];

    /****************************** SO-TASK-5 *******************************/
    decrease_tokens(proc_nr_n); /* taking tokens away from process */
    /************************************************************************/

    if (rmp->priority < MIN_USER_Q) {
        rmp->priority += 1; /* lower priority */
    }

    if ((rv = schedule_process_local(rmp)) != OK) {
        return rv;
    }

    /****************************** SO-TASK-5 *******************************/
    distribute_tokens((global_systime_difference() * SCHED_FACTOR));
    /************************************************************************/

    return OK;
}

/*===========================================================================*
 *				do_stop_scheduling			     *
 *===========================================================================*/
int do_stop_scheduling(message *m_ptr)
{
    register struct schedproc *rmp;
    int proc_nr_n;

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    if (sched_isokendpt(m_ptr->m_lsys_sched_scheduling_stop.endpoint,
                        &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OOQ msg "
                       "%d\n", m_ptr->m_lsys_sched_scheduling_stop.endpoint);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];
#ifdef CONFIG_SMP
    cpu_proc[rmp->cpu]--;
#endif
    rmp->flags = 0; /*&= ~IN_USE;*/

    return OK;
}

/*===========================================================================*
 *				do_start_scheduling			     *
 *===========================================================================*/
int do_start_scheduling(message *m_ptr)
{
    register struct schedproc *rmp;
    int rv, proc_nr_n, parent_nr_n;

    /* we can handle two kinds of messages here */
    assert(m_ptr->m_type == SCHEDULING_START ||
           m_ptr->m_type == SCHEDULING_INHERIT);

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    /* Resolve endpoint to proc slot. */
    if ((rv = sched_isemtyendpt(m_ptr->m_lsys_sched_scheduling_start.endpoint,
                                &proc_nr_n)) != OK) {
        return rv;
    }
    rmp = &schedproc[proc_nr_n];

    /****************************** SO-TASK-5 ******************************/
    rmp->tokens = MAX_TOKENS;
    rmp->tokens_sign = POSITIVE;

    clock_t user_time, sys_time, up_time;
    time_t boot_time;
    sys_times(rmp->endpoint, &user_time, &sys_time, &up_time, &boot_time);

    rmp->old_systime = up_time;
    /***********************************************************************/

    /* Populate process slot */
    rmp->endpoint     = m_ptr->m_lsys_sched_scheduling_start.endpoint;
    rmp->parent       = m_ptr->m_lsys_sched_scheduling_start.parent;
    rmp->max_priority = m_ptr->m_lsys_sched_scheduling_start.maxprio;
    rmp->tokens = MAX_TOKENS; 						/* SO-TASK-5 */
    if (rmp->max_priority >= NR_SCHED_QUEUES) {
        return EINVAL;
    }

    /* Inherit current priority and time slice from parent. Since there
     * is currently only one scheduler scheduling the whole system, this
     * value is local and we assert that the parent endpoint is valid */
    if (rmp->endpoint == rmp->parent) {
        /* We have a special case here for init, which is the first
           process scheduled, and the parent of itself. */
        rmp->priority   = USER_Q;
        rmp->time_slice = DEFAULT_USER_TIME_SLICE;

        /*
         * Since kernel never changes the cpu of a process, all are
         * started on the BSP and the userspace scheduling hasn't
         * changed that yet either, we can be sure that BSP is the
         * processor where the processes run now.
         */
#ifdef CONFIG_SMP
        rmp->cpu = machine.bsp_id;
		/* FIXME set the cpu mask */
#endif
    }

    switch (m_ptr->m_type) {

        case SCHEDULING_START:
            /* We have a special case here for system processes, for which
             * quanum and priority are set explicitly rather than inherited
             * from the parent */
            rmp->priority   = rmp->max_priority;
            rmp->time_slice = m_ptr->m_lsys_sched_scheduling_start.quantum;
            break;

        case SCHEDULING_INHERIT:
            /* Inherit current priority and time slice from parent. Since there
             * is currently only one scheduler scheduling the whole system, this
             * value is local and we assert that the parent endpoint is valid */
            if ((rv = sched_isokendpt(m_ptr->m_lsys_sched_scheduling_start.parent,
                                      &parent_nr_n)) != OK)
                return rv;

            rmp->priority = schedproc[parent_nr_n].priority;
            rmp->time_slice = schedproc[parent_nr_n].time_slice;
            break;

        default:
            /* not reachable */
            assert(0);
    }

    /* Take over scheduling the process. The kernel reply message populates
     * the processes current priority and its time slice */
    if ((rv = sys_schedctl(0, rmp->endpoint, 0, 0, 0)) != OK) {
        printf("Sched: Error taking over scheduling for %d, kernel said %d\n",
               rmp->endpoint, rv);
        return rv;
    }
    rmp->flags = IN_USE;

    /* Schedule the process, giving it some quantum */
    pick_cpu(rmp);
    while ((rv = schedule_process(rmp, SCHEDULE_CHANGE_ALL)) == EBADCPU) {
        /* don't try this CPU ever again */
        cpu_proc[rmp->cpu] = CPU_DEAD;
        pick_cpu(rmp);
    }

    if (rv != OK) {
        printf("Sched: Error while scheduling process, kernel replied %d\n",
               rv);
        return rv;
    }

    /* Mark ourselves as the new scheduler.
     * By default, processes are scheduled by the parents scheduler. In case
     * this scheduler would want to delegate scheduling to another
     * scheduler, it could do so and then write the endpoint of that
     * scheduler into the "scheduler" field.
     */

    m_ptr->m_sched_lsys_scheduling_start.scheduler = SCHED_PROC_NR;

    return OK;
}

/*===========================================================================*
 *				do_nice					     *
 *===========================================================================*/
int do_nice(message *m_ptr)
{
    struct schedproc *rmp;
    int rv;
    int proc_nr_n;
    unsigned new_q, old_q, old_max_q;

    /* check who can send you requests */
    if (!accept_message(m_ptr))
        return EPERM;

    if (sched_isokendpt(m_ptr->m_pm_sched_scheduling_set_nice.endpoint, &proc_nr_n) != OK) {
        printf("SCHED: WARNING: got an invalid endpoint in OoQ msg "
                       "%d\n", m_ptr->m_pm_sched_scheduling_set_nice.endpoint);
        return EBADEPT;
    }

    rmp = &schedproc[proc_nr_n];
    new_q = m_ptr->m_pm_sched_scheduling_set_nice.maxprio;
    if (new_q >= NR_SCHED_QUEUES) {
        return EINVAL;
    }

    /* Store old values, in case we need to roll back the changes */
    old_q     = rmp->priority;
    old_max_q = rmp->max_priority;

    /* Update the proc entry and reschedule the process */
    rmp->max_priority = rmp->priority = new_q;

    if ((rv = schedule_process_local(rmp)) != OK) {
        /* Something went wrong when rescheduling the process, roll
         * back the changes to proc struct */
        rmp->priority     = old_q;
        rmp->max_priority = old_max_q;
    }

    return rv;
}

/*===========================================================================*
 *				schedule_process			     *
 *===========================================================================*/
static int schedule_process(struct schedproc * rmp, unsigned flags)
{
    int err;
    int new_prio, new_quantum, new_cpu;

    pick_cpu(rmp);

    if (flags & SCHEDULE_CHANGE_PRIO)
        new_prio = rmp->priority;
    else
        new_prio = -1;

    if (flags & SCHEDULE_CHANGE_QUANTUM)
        new_quantum = rmp->time_slice;
    else
        new_quantum = -1;

    if (flags & SCHEDULE_CHANGE_CPU)
        new_cpu = rmp->cpu;
    else
        new_cpu = -1;

    if (rmp->tokens_sign == POSITIVE) {
        /* call sys_schedule only if process has positive amount of tokens */
        if ((err = sys_schedule(rmp->endpoint, new_prio,
                                new_quantum, new_cpu)) != OK) {
            printf("PM: An error occurred when trying to schedule %d: %d\n",
                   rmp->endpoint, err);
        }
    }

    return err;
}


/*===========================================================================*
 *				start_scheduling			     *
 *===========================================================================*/

void init_scheduling(void)
{
    balance_timeout = BALANCE_TIMEOUT * sys_hz();
    init_timer(&sched_timer);
    set_timer(&sched_timer, balance_timeout, balance_queues, 0);

    /*********************** SO-TASK-5 **************************************/
    sys_times(NONE, NULL, NULL, &last_refill_time, NULL);
    /************************************************************************/

}

/*===========================================================================*
 *				balance_queues				     *
 *===========================================================================*/

/* This function in called every 100 ticks to rebalance the queues. The current
 * scheduler bumps processes down one priority when ever they run out of
 * quantum. This function will find all proccesses that have been bumped down,
 * and pulls them back up. This default policy will soon be changed.
 */
static void balance_queues(minix_timer_t *tp)
{
    struct schedproc *rmp;
    int proc_nr;

    /*********************** SO-TASK-5 **************************************/
    distribute_tokens((global_systime_difference() * SCHED_FACTOR));
    /************************************************************************/

    for (proc_nr=0, rmp=schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
        if (rmp->flags & IN_USE) {
            if (rmp->priority > rmp->max_priority) {
                rmp->priority -= 1; /* increase priority */

                schedule_process_local(rmp);
            }
            /****************************************************************/
        }
    }

    set_timer(&sched_timer, balance_timeout, balance_queues, 0);
}