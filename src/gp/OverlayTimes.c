/*
 * Overlay times widget.  This is where plots can write per-component data
 * details like exact data time, altitude, etc.
 */
static char *rcsid = "$Id: OverlayTimes.c,v 2.2 1994-04-15 21:26:17 burghart Exp $";
/*		Copyright (C) 1994 by UCAR
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
# include <X11/Shell.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/AsciiText.h>

# include <defs.h>
# include <pd.h>
# include "GraphProc.h"

static Widget	OTWidget = NULL;
static char	OTString[1024];

/*
 * Prototypes
 */
static Widget	ot_Create FP ((char *junk, Widget parent, XtAppContext appc));
static void	ot_Update FP ((void));




void
ot_Init ()
/*
 * Declare the overlay times widget to UI, so that it can create it when
 * requested.
 */
{
	char	title[40];

	sprintf (title, "Data times for %s", Ourname);
	uw_def_widget ("overlay", title, ot_Create, 0, 0);
}




static Widget
ot_Create (junk, parent, appc)
char *junk;
Widget parent;
XtAppContext appc;
/*
 * Actually create the overlay status widget.
 */
{
	Arg	args[10];
	int	n;
/*
 * Create an AsciiText widget to hold our info.  The "resizable" resource
 * is a constraint resource available to us because our parent is a Form
 * widget.  We set it to let the form know we may be resizing.
 */
	n = 0;
	XtSetArg (args[n], XtNstring, "nothing"); n++;
	XtSetArg (args[n], XtNeditType, XawtextEdit); n++;
	XtSetArg (args[n], XtNresize, XawtextResizeBoth); n++;
	XtSetArg (args[n], XtNresizable, True); n++;
	OTWidget = XtCreateManagedWidget ("OverlayTimes", asciiTextWidgetClass,
					  parent, args, n);
/*
 * We know that UI will create us with a Form widget parent and a TopLevelShell
 * as a grandparent.  We *need* a resizable shell, so we force it..
 */
	n = 0;
	XtSetArg (args[n], XtNallowShellResize, True); n++;
	XtSetValues (XtParent (parent), args, n);
/*
 * Put in the current string
 */
	ot_Update ();

	return (OTWidget);
}




void
ot_SetString (text)
char *text;
/*
 * Set a new string for the widget.
 */
{
	strcpy (OTString, text);
	ot_Update ();
}




void 
ot_Append (text)
char *text;
/*
 * Append this string to our status.
 */
{
	strcat (OTString, text);
	ot_Update ();
}




void
ot_AddStatusLine (comp, plat, fname, t)
char	*comp, *plat, *fname;
ZebTime *t;
/*
 * Add a "standard" status line to the overlay times widget with component, 
 * platform, field, and time.
 */
{
	sprintf (OTString + strlen (OTString), "%-14s ", comp);
	sprintf (OTString + strlen (OTString), "%-10s ", plat);
	sprintf (OTString + strlen (OTString), "%-10s ", fname);

	TC_EncodeTime (t, TC_Full, OTString + strlen (OTString));
	strcat (OTString, "\n");

	ot_Update ();
}




char *
ot_GetString ()
/*
 * Return the current status string.
 */
{
	return (OTString);
}




static void
ot_Update ()
/*
 * Display the updated text
 */
{
	Arg	arg;
	char	*current;
	XawTextBlock	xtext;

	if (! OTWidget)
		return;
/*
 * Using XawTextReplace() seems to be the only way to get the resize of
 * the AsciiTextWidget to work properly.
 */
	XtSetArg (arg, XtNstring, &current);
	XtGetValues (OTWidget, &arg, 1);

	xtext.firstPos = 0;
	xtext.length = strlen (OTString);
	xtext.ptr = OTString;
	xtext.format = FMT8BIT;

	XawTextReplace (OTWidget, 0, strlen (current), &xtext);
	XawTextInvalidate (OTWidget, 0, strlen (OTString) + 1);
}