/*
 * Low-level (FD) event management routines.
 */
# include <sys/types.h>
# include <sys/time.h>
# include <errno.h>

# include "../include/defs.h"
# include "EventQueue.h"
# include "LLEvent.h"

static char *rcsid = "$Id: LLEvent.c,v 1.2 1990-06-11 14:24:34 corbet Exp $";


/*
 * The master fd_set, which always holds the list of FD's that we are watching.
 */
static fd_set Mfd;
static int Mfd_width = 0;

typedef void (*vfptr) ();
static vfptr Procs[256];


void
lle_AddFD (fd, proc)
int fd;
void (*proc) ();
/*
 * Add a file descriptor to the list of those being watched.
 * Entry:
 *	FD	is the FD of interest;
 *	PROC	is the procedure to call when there is input on FD.
 */
{
	if (Mfd_width == 0)
		FD_ZERO (&Mfd);
	FD_SET (fd, &Mfd);
	Procs[fd] = proc;
	if (Mfd_width < fd)
		Mfd_width = fd;
}





lle_MainLoop ()
/*
 * Run the main loop.
 */
{
	int did_work = TRUE;

	for (;;)
	{
	/*
	 * Check for input, blocking if there was nothing left in the
	 * event queue.
	 */
		lle_DoInput (! did_work);
	/*
	 * Clear out the event queues.
	 */
		did_work = Eq_Execute ();
	}
}





lle_DoInput (block)
int block;
/*
 * Deal with our input streams.  We will block waiting for input iff
 * "block" is true.
 */
{
	int status, i, did_work = TRUE;
	fd_set fds;
	static struct timeval noblock = { 0, 0 };
/*
 * Wait for something.
 */
	fds = Mfd;
	while ((status = select (Mfd_width + 1, &fds, 0, 0,
		block ? (struct timeval *) 0 : &noblock)) < 0)
	{
	/*
	 * Just do another select if we were stopped by
	 * a signal
	 */
		if (errno == EINTR)
			continue;
	/*
	 * Bad error, shut down
	 */
		msg_log ("Select error %d", errno);
		GPShutDown ();
	}
/*
 * Deal with what came in.
 */
	for (i = 0; status && i <= Mfd_width; i++)
		if (FD_ISSET (i, &fds))
		{
			status--;
			(*Procs[i]) (i);
		}
}
