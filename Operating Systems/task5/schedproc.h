/* This table has one slot per process.  It contains scheduling information
 * for each process.
 */
#include <limits.h>

#include <minix/bitmap.h>

/* EXTERN should be extern except in main.c, where we want to keep the struct */
#ifdef _MAIN
#undef EXTERN
#define EXTERN
#endif

#ifndef CONFIG_SMP
#define CONFIG_MAX_CPUS 1
#endif

/**
 * We might later want to add more information to this table, such as the
 * process owner, process group or cpumask.
 */

/************************* SO-TASK-5 ***********************************/
/* defines for tokens_sign variable */

#define NEGATIVE -1
#define POSITIVE 1
/***********************************************************************/

EXTERN struct schedproc {
    endpoint_t endpoint;	/* process endpoint id */
    endpoint_t parent;	/* parent endpoint id */
    unsigned flags;		/* flag bits */

    /* User space scheduling */
    unsigned max_priority;	/* this process' highest allowed priority */
    unsigned priority;		/* the process' current priority */
    unsigned time_slice;		/* this process's time slice */
    unsigned cpu;		/* what CPU is the process running on */
    bitchunk_t cpu_mask[BITMAP_CHUNKS(CONFIG_MAX_CPUS)]; /* what CPUs is the
								process allowed
								to run on */

    /************************* SO-TASK-5 ***********************************/
    clock_t old_systime;
    clock_t tokens;
    int tokens_sign; /* implementation of clock_t in Minix as unsigned int
 	involves special way of adding/subtracting tokens pool in decrease_tokens()
 	and distribute_tokens() */
    /***********************************************************************/

} schedproc[NR_PROCS];

/* Flag values */
#define IN_USE		0x00001	/* set when 'schedproc' slot in use */