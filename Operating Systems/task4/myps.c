#include "pm.h"
#include <sys/resource.h>
#include <sys/stat.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/endpoint.h>
#include <fcntl.h>
#include <signal.h>		/* needed only because mproc.h needs it */
#include "mproc.h"

#include <minix/config.h>
#include <minix/timers.h>
#include <machine/archtypes.h>

int do_myps (void) {
	int uid = m_in.m1_i1;

	register struct mproc *rmp;

	for (rmp = &mproc[0]; rmp < &mproc[NR_PROCS]; rmp++) {
		if (rmp->mp_realuid == uid && (rmp->mp_flags & IN_USE) == IN_USE) {
			printf("%d\n", rmp->mp_pid);
			printf("%d\n", mproc[rmp->mp_parent].mp_pid);
			printf("%d\n", uid);
			printf("\n");
		}
	}
	
	return 0;	/* sukces */
}









// 	int old_realuid = 0, old_ppid = 0, old_pid = 0;

// 		if (uid == old_realuid && rmp->mp_pid == old_pid && 
// mproc[rmp->mp_parent].mp_pid == old_ppid)
// 			break;

// old_realuid = uid;
// 		old_pid = rmp->mp_pid;
// 		old_ppid = mproc[rmp->mp_parent].mp_pid;