/*
 * Figure out how to do the data access.
 */

/*		Copyright (C) 1987,88,89,90,91 by UCAR
 *	University Corporation for Atmospheric Research
 *		   All rights reserved
 *
 * No part of this work covered by the copyrights herein may be reproduced
 * or used in any form or by any means -- graphic, electronic, or mechanical,
 * including photocopying, recording, taping, or information storage and
 * retrieval systems -- without permission of the copyright owner.
 * 
 * This software and any accompanying written materials are provided "as is"
 * without warranty of any kind.  UCAR expressly disclaims all warranties of
 * any kind, either express or implied, including but not limited to the
 * implied warranties of merchantibility and fitness for a particular purpose.
 * UCAR does not indemnify any infringement of copyright, patent, or trademark
 * through use or modification of this software.  UCAR does not provide 
 * maintenance or updates for its software.
 */
# include <stdio.h>

# include <defs.h>
# include <message.h>
# include "DataStore.h"
# include "dsPrivate.h"
# include "GetList.h"
# include "dslib.h"

RCSID("$Id: GetList.c,v 3.13 1999-03-01 02:03:33 burghart Exp $")

/*
 * Getlist lookaside list.
 */
static GetList *GList = 0;


static GetList	*dgl_GetEntry (void);
static void	dgl_ReturnEntry (GetList *);
static int	dgl_DoList (GetList *, const DataFile *);
static int	dgl_Overlaps (GetList *, const DataFile *);
static GetList	*dgl_FixList (GetList *, const DataFile *, int *);




static GetList *
dgl_GetEntry ()
/*
 * Get a free getlist entry.
 */
{
	GetList *ret;

	if (GList)
	{
		ret = GList;
		GList = ret->gl_next;
	}
	else
		ret = ALLOC (GetList);

	ret->gl_flags = 0;
	return (ret);
}



static void
dgl_ReturnEntry (gl)
GetList *gl;
/*
 * Return this getlist entry, which means we're through with any file
 * that it pointed to.
 */
{
	gl->gl_next = GList;
	GList = gl;
}



void 
dgl_ForceClosure()
/*
 * Release our free chain
 */
{
	GetList *gl, *next;

	gl = GList;
	while (gl)
	{
		next = gl->gl_next;
		free (gl);
		gl = next;
	}
	GList = NULL;
}



void
dgl_ReturnList (gl)
GetList *gl;
/* 
 * Return this entire list.
 */
{
	GetList *zap;
	while (gl)
	{
		zap = gl;
		gl = gl->gl_next;
		dgl_ReturnEntry (zap);
	}
}



GetList *
dgl_MakeGetList (pid, begin, end)
PlatformId pid;
ZebTime *begin, *end;
/*
 * Figure out how to satisfy this data object.
 */
{
	GetList *list, *l, *zap;
/*
 * Make an initial, unsatisfied entry.
 */
	list = dgl_GetEntry ();
	list->gl_flags = 0;
	list->gl_begin = *begin;	/* times are initially inclusive */
	list->gl_flags |= GLF_BEG_INCL;
	list->gl_end = *end;
	list->gl_flags |= GLF_END_INCL;
	list->gl_next = NULL;
/*
 * Now try to satisfy it against the platform lists.
 */
/* XXX Searches all sources by default for now */
	dgl_DoList (list, ds_FindDFBefore (SRC_ALL, pid, end));
/*
 * Remove any unsatisfied elements at the beginning of the list.
 */
	while (list && ! (list->gl_flags & GLF_SATISFIED))
	{
		zap = list;
		list = list->gl_next;
		dgl_ReturnEntry (zap);
	}
/*
 * Now wander into the middle of the list and look for unsatisfied chunks
 * therein.
 */
	for (l = zap = list; zap; zap = l->gl_next)
	{
		if (! (zap->gl_flags & GLF_SATISFIED))
		{
			l->gl_next = zap->gl_next;
			dgl_ReturnEntry (zap);
		}
		else
			l = zap;
	}
/*
 * Return what's left, if anything.
 */
	return (list);
}





static int
dgl_DoList (GetList *list, const DataFile *df)
/*
 * Try to satisfy this getlist against the given data file list.
 */
{
	int ret = TRUE;
	GetList *lp;
/*
 * Mark all list entries as not having been tried.
 */
	if (! df)
		return (FALSE);
	for (lp = list; lp; lp = lp->gl_next)
		lp->gl_flags &= ~GLF_TRIED;
/*
 * Now pass through the list and try to satisfy things.
 */
	while (list)
	{
	/*
	 * Find the first unsatisfied, untried getlist entry.  If there are no
	 * more, we're done.
	 */
		for (; list; list = list->gl_next)
			if (! (list->gl_flags & (GLF_SATISFIED | GLF_TRIED)))
				break;
		if (! list)
			return (ret);
	/*
	 * Move forward through the data list until we find an entry which
	 * overlaps with this getlist entry.
	 * MAKE THIS SMARTER!
	 * (Smarter means not passing over the files that we have already
	 *  been over N times satisfying earlier getlist entries.)
	 */
		do {
			if (dgl_Overlaps (list, df))
			{
				break;  /* No point looking further back*/
			}
			if (TC_Less (df->df_core.dfc_end, list->gl_begin))
			{
				df = 0;	/* Never will find it here */
				break;
			}
			df = ds_PrevFile (df);
		}
	 	while (df);
	/*
	 * If we found nothing, this entry can not be satisfied, so we bail.
	 */
	 	if (! df)
		{
			list->gl_flags |= GLF_TRIED;
			list = list->gl_next;
			ret = FALSE;
		}
	/*
	 * We found something.  Rework the list as appropriate.
	 */
	 	else
			list = dgl_FixList (list, df, &ret);
	}
	return (ret);
}





static int 
dgl_Overlaps (GetList *gp, const DataFile *df)
/*
 * See if these two entries overlap in time.
 */
{
/*
 * If the data begins before our desired time, then we have an overlap iff
 * it ends after that time.
 */
	if  (TC_Less (df->df_core.dfc_begin, gp->gl_begin))
		return ((gp->gl_flags & GLF_BEG_INCL) ?
			TC_LessEq (gp->gl_begin, df->df_core.dfc_end) :
			TC_Less (gp->gl_begin, df->df_core.dfc_end));
/*
 * Otherwise overlap iff the data begins before the desired end time,
 * or if the end time is inclusive, at the end time.
 */
	else
		return ((gp->gl_flags & GLF_END_INCL) ? 
			TC_LessEq (df->df_core.dfc_begin, gp->gl_end) : 
			TC_Less (df->df_core.dfc_begin, gp->gl_end));
}





static GetList *
dgl_FixList (GetList *gp, const DataFile *df, int *complete)
/*
 * Fix up the getlist to reflect what this datafile can do for us.  The
 * current entry is split if necessary.  COMPLETE is set FALSE iff there
 * is unsatisfied data at the front end of things.
 */
{
	GetList *new;
/*
 * If the data ends before our request, we have to split the request.
 */
	if (TC_Less (df->df_core.dfc_end, gp->gl_end))
	{
		*complete = FALSE;
	/*
	 * Get a new entry and splice it into the list.
	 */
		new = dgl_GetEntry ();
		*new = *gp;
		gp->gl_next = new;
		gp->gl_flags |= GLF_TRIED;	/* No point in trying again */
	/*
	 * Now fix up the times and move past the unsatisfiable piece.
	 */
	 	gp->gl_begin = df->df_core.dfc_end;
		gp->gl_flags &= ~GLF_BEG_INCL;
		new->gl_end = df->df_core.dfc_end;
		new->gl_flags |= GLF_END_INCL;
		gp = new;
	}
/*
 * At this point, we know we can satisfy the current entry to the end.
 * Mark it accordingly.  If we can go all the way back to the beginning, 
 * we're done.
 */
	gp->gl_df = *df;
	gp->gl_flags |= GLF_SATISFIED;
	if (TC_LessEq (df->df_core.dfc_begin, gp->gl_begin))
		return (gp);
/*
 * Otherwise we have to split again, and leave an unsatisfied chunk for
 * the next iteration.
 */
	new = ALLOC (GetList);
	*new = *gp;
	new->gl_next = gp->gl_next;
	gp->gl_next = new;
	gp->gl_begin = df->df_core.dfc_begin;
	gp->gl_flags |= GLF_BEG_INCL;
	new->gl_end = df->df_core.dfc_begin;
	new->gl_flags &= ~GLF_END_INCL;
	new->gl_flags &= ~GLF_SATISFIED;
	return (new);
}
