/*
 * The graphics process event/processing queue system.
 */
static char *rcsid = "$Id: EventQueue.c,v 1.3 1991-02-20 18:26:05 corbet Exp $";

# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include "../include/defs.h"
# include "../include/pd.h"
# include "../include/message.h"
# include "GraphProc.h"
# include "EventQueue.h"

/*
 * The event queue consists of actions to be performed -- as represented by
 * procedures to call -- and associated priorities.  High priority actions
 * are executed before lower priority ones.
 */


/*
 * An entry in the priority queue.
 */
struct pq_entry
{
	void	(*pqe_proc) ();		/* The procedure to call	*/
	char	*pqe_data;		/* Proc data			*/
	int	pqe_len;		/* Length of that data		*/
	struct pq_entry *pqe_next;	/* Next entry			*/
};

/*
 * The actual priority queue itself.
 */
static struct pq_entry *P_queue[3] = { 0, 0, 0};
static struct pq_entry *P_tails[3];
static struct pq_entry *P_free = 0;	/* pqe free list		*/

/*
 * If HoldProcess is nonzero, only stuff at the PUrgent level will be
 * done.
 */
static int HoldProcess = 0;
static Pixel OldBorder;		/* For when we change it		*/


/*
 * Forward routine definitions.
 */
# ifdef __STDC__
static struct pq_entry *Eq_NewEntry (void (*proc) (), char *data, int len);
static void Eq_RemoveEntry (EQpriority pri, struct pq_entry *pqe);
static void Eq_FreeEntry (struct pq_entry *pqe);
static void Eq_RestoreBorder (void);
# else
static struct pq_entry *Eq_NewEntry ();
static void Eq_RemoveEntry ();
static void Eq_FreeEntry ();
static void Eq_RestoreBorder ();
# endif





void
Eq_AddEvent (pri, proc, data, len, override)
EQpriority pri;
void (*proc) ();
void *data;
int len;
EQoverride override;
/*
 * Add an event to the event queue.
 * Entry:
 *	PRI	is the priority of this event;
 *	PROC	is the procedure to execute this event;
 *	DATA	is the data to pass to the procedure;
 *	LEN	is the length of that data.
 * Exit:
 *	The action has been queued to be performed.
 */
{
	struct pq_entry *pqe, *ent;
/*
 * Handle overriding.
 */
	if (override == Override)
		Eq_ZapProc (pri, proc);
	else if (override == Bounce)
		for (ent = P_queue[pri]; ent; ent = ent->pqe_next)
			if (ent->pqe_proc == proc)
				return;
/*
 * Get a new queue entry and fill it in.
 */
	pqe = Eq_NewEntry (proc, data, len);
/*
 * Add it to the proper queue.
 */
	if (! P_queue[pri])
		P_queue[pri] = pqe;
	else
		P_tails[pri]->pqe_next = pqe;
	P_tails[pri] = pqe;
	pqe->pqe_next = 0;
}




void
Eq_ZapProc (pri, proc)
EQpriority pri;
void (*proc) ();
/*
 * Get rid of all entries involving this proc.
 */
{
	struct pq_entry *ent;

	for (ent = P_queue[pri]; ent; )
	{
		if (ent->pqe_proc == proc)
		{
			struct pq_entry *zap = ent;
			ent = ent->pqe_next;
			Eq_RemoveEntry (pri, zap);
		}
		else
			ent = ent->pqe_next;
	}
}





static struct pq_entry *
Eq_NewEntry (proc, data, len)
void (*proc) ();
char *data;
int len;
/*
 * Get a new priority queue entry.
 */
{
	struct pq_entry *new;
/*
 * Pull an entry off the free list if possible.
 */
	if (P_free)
	{
		new = P_free;
		P_free = new->pqe_next;
	}
	else
		new = ALLOC (struct pq_entry);
/*
 * Go ahead and fill in the data.
 */
	new->pqe_proc = proc;
	new->pqe_len = len;
	if (len)
	{
		new->pqe_data = malloc (len);
		memcpy (new->pqe_data, data, len);
	}
	return (new);
}





static void
Eq_RemoveEntry (pri, pqe)
EQpriority pri;
struct pq_entry *pqe;
/*
 * Remove this entry from a queue.
 */
{
	struct pq_entry *last;
/*
 * If this is the first entry, clear it directly.
 */
	if (P_queue[pri] == pqe)
	{
		P_queue[pri] = P_queue[pri]->pqe_next;
		Eq_FreeEntry (pqe);
		return;
	}
/*
 * Otherwise we must search for it.
 */
	for (last = P_queue[pri]; last && last->pqe_next != pqe;
			last = last->pqe_next)
		; /* yawn */
	if (! last)
	{
		msg_log ("WTH??? Missing PQE in Eq_RemoveEntry");
		return;
	}
/*
 * Splice it out of the list.
 */
	last->pqe_next = pqe->pqe_next;
	if (P_tails[pri] == pqe)
		P_tails[pri] = last;
/*
 * Get rid of the entry.
 */
	Eq_FreeEntry (pqe);
}





static void
Eq_FreeEntry (pqe)
struct pq_entry *pqe;
/*
 * Get rid of this entry.
 */
{
	if (pqe->pqe_len)
		free (pqe->pqe_data);
	pqe->pqe_next = P_free;
	P_free = pqe;
}




int
Eq_Execute ()
/*
 * Execute an event from the queue.  Returns TRUE iff there was something
 * to do.
 */
{
	register struct pq_entry *pqe;
	EQpriority pri;
/*
 * See if there is work to do.
 */
	if (pqe = P_queue[PUrgent])
		pri = PUrgent;
	else if (! HoldProcess && (pqe = P_queue[PDisplay]))
		pri = PDisplay;
	else if (! HoldProcess && (pqe = P_queue[PWhenever]))
		pri = PWhenever;
/*
 * Do it.
 */
	if (pqe)
	{
		(*pqe->pqe_proc) (pqe->pqe_data, pqe->pqe_len);
		Eq_RemoveEntry (pri, pqe);
		return (1);
	}
	return (0);
}




void
Eq_HoldProcess ()
/*
 * Put all but the most urgent stuff on hold.
 */
{
	Arg args[2];
	XColor xc;

	if (++HoldProcess == 1)
	{
		XtSetArg (args[0], XtNborderColor, &OldBorder);
		XtGetValues (GrShell, args, 1);
		ct_GetColorByName ("red", &xc);
		XtSetArg (args[0], XtNborderColor, xc.pixel);
		XtSetValues (GrShell, args, 1);
	}
}



void
Eq_ReleaseHold ()
/*
 * Release a process hold that had been in effect.
 */
{
	if (HoldProcess > 0)
	{
		if (--HoldProcess == 0)
			Eq_RestoreBorder ();
	}
	else
		msg_ELog (EF_PROBLEM, "Decrement on zero HoldProcess");
}



void
Eq_BreakHold ()
/*
 * Break out of a hold process forcibly.
 */
{
/*
 * If there is a hold in effect, cancel it.  Also cancel any user event
 * override that may be in effect.
 */
	if (HoldProcess)
	{
		msg_ELog (EF_INFO, "HoldProcess (%d) broken", HoldProcess);
		HoldProcess = 0;
		Ue_ResetOverride ();
		Eq_RestoreBorder ();
	}
}




static void
Eq_RestoreBorder ()
/*
 * Put the border color back.
 */
{
	Arg args[2];

	XtSetArg  (args[0], XtNborderColor, OldBorder);
	XtSetValues (GrShell, args, 1);
}
