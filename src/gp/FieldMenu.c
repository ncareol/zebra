/*
 * The available field menu.  This is a carved up version of the data 
 * available menu....
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
# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <X11/Xaw/SimpleMenu.h>
# include <X11/Xaw/SmeBSB.h>
# include <X11/Shell.h>
# include <X11/Xaw/SmeLine.h>

# include <defs.h>
# include <message.h>
# include <pd.h>
# include <DataStore.h>
# include <ui_date.h>
# include "GraphProc.h"
MAKE_RCSID ("$Id: FieldMenu.c,v 2.1 1993-07-16 16:05:54 corbet Exp $")


/*
 * Stuff for the SME menu.
 */
# define MAXENTRY 40
static Widget Menu, Entries[MAXENTRY];
static char Platform[40];	/* Platform of interest	*/
static FieldId Fields[MAXENTRY];
static int NManaged;

static stbl VTable;

static char IComp[60];

static void EntryCallback FP ((Widget, XtPointer, XtPointer));
static void PopupCallback FP ((Widget, XtPointer, XtPointer));
static int SetupFields FP ((void));
static int Funky FP ((char *));
static int AddPlatform FP ((char *, int, ZebTime *));
static void ToRealTime FP ((Widget, XtPointer, XtPointer));



void
InitFieldMenu ()
/*
 * Get the field menu set up.
 */
{
	Arg args[2];
	int i, n;
	Widget rt;

	VTable = usy_g_stbl ("ui$variable_table");
/*
 * Create a shell for the thing.
 */
	XtSetArg (args[0], XtNlabel, "Field selection menu");
	Menu = XtCreatePopupShell ("FieldMenu", simpleMenuWidgetClass,
		Top, args, 1);
	XtAddCallback (Menu, XtNpopupCallback, (XtCallbackProc) PopupCallback, 
		(XtPointer) Menu);
	XtCreateManagedWidget ("Line", smeLineObjectClass, Menu, NULL, 0);
# ifdef notdef
/*
 * Real time mode.
 */
	XtSetArg (args[0], XtNlabel, "Real time mode");
	rt = XtCreateManagedWidget ("realtime", smeBSBObjectClass, Menu,
		args, 1);
	XtAddCallback (rt, XtNcallback, (XtCallbackProc) ToRealTime, 
		(XtPointer) 0);
	XtCreateManagedWidget ("Line", smeLineObjectClass, Menu, NULL, 0);
# endif
/*
 * Create all of the entries, but don't manage them now.
 */
	n = 0;
	XtSetArg (args[0], XtNlabel, "(nuttin)");	n++;
	for (i = 0; i < MAXENTRY; i++)
	{
		Entries[i] = XtCreateWidget ("DAEntry", smeBSBObjectClass,
			Menu, args, 1);
		XtAddCallback (Entries[i], XtNcallback, 
			(XtCallbackProc) EntryCallback, (XtPointer) i);
	}
	NManaged = 0;
}






/* ARGSUSED */
static void
EntryCallback (w, xwhich, junk)
Widget w;
XtPointer xwhich, junk;
/*
 * One of the entries has been selected.
 */
{
	int which = (int) xwhich;
	char cbuf[200];
	UItime uitime;
/*
 * Here we just put together the command and go.  Start by searching for
 * a command to execute.
 */
# ifdef notdef
	if (! pd_Retrieve (Pd, IComp, "platform", plat, SYMT_STRING))
		qual = NULL;
# endif
	if (! pda_Search (Pd, IComp, "field-select-command", Platform, 
			cbuf, SYMT_STRING))
	{
		parameter (IComp, "field", F_GetName (Fields[which]));
		return;
	}
/*
 * Now format the rest of the command with the field.
 */
	strcat (cbuf, " ");
	strcpy (cbuf + strlen (cbuf), F_GetName (Fields[which]));

	msg_ELog (EF_DEBUG, "FMenu cmd '%s'", cbuf);
	ui_perform (cbuf);
}





static void
PopupCallback (w, junk, junk1)
Widget w;
XtPointer junk, junk1;
/*
 * We're being popped up.  Set the entries accordingly.
 */
{
	int nentry, i;
	Arg args[2];
	char string[80];
	UItime uitime;
/*
 * Get the platforms set.
 */
	nentry = SetupFields ();
/*
 * Go through and make the labels for each one.
 */
	for (i = 0; i < nentry; i++)
	{
	/*
	 * Add the text.
	 */
		sprintf (string, "%s", F_GetDesc (Fields[i]));
		if (strcmp (string, F_GetName (Fields[i])))
			sprintf (string + strlen (string), " (%s)",
						F_GetName (Fields[i]));
		XtSetArg (args[0], XtNlabel, string);
		XtSetValues (Entries[i], args, 1);
	/*
	 * If this one isn't managed yet, make it so now.
	 */
	 	if (i >= NManaged)
		{
			XtManageChild (Entries[i]);
			NManaged++;
		}
	}
/*
 * Clean out extras if need be.
 */
 	for (i = nentry; i < NManaged; i++)
		XtUnmanageChild (Entries[i]);
	NManaged = nentry;
}





static int
SetupFields ()
/*
 * Figure out what should appear in this menu where.
 */
{
	SValue v;
	int type, nfield = MAXENTRY;
	PlatformId pid;
/*
 * See which is our component and platform.
 */
	if (! usy_g_symbol (VTable, "icon_component", &type, &v))
		return (Funky ("no icon component"));
	strcpy (IComp, v.us_v_ptr);
	if (! pd_Retrieve (Pd, IComp, "platform", Platform, SYMT_STRING))
		return (Funky ("No platform!"));
/*
 * Query the fields that are available.
 */
	if ((pid = ds_LookupPlatform (Platform)) == BadPlatform)
		return (Funky ("Bad platform name"));
	ds_GetFields (pid, &PlotTime, &nfield, Fields);
	return (nfield);
}





static int
Funky (s)
char *s;
/*
 * Return such that the given gripe appears in the menu.
 */
{
	msg_ELog (EF_PROBLEM, "Field menu problem: %s", s);
	return (0);
}
