/*
 * Our status widget.
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

static char *rcsid = "$Id: StatusWidget.c,v 2.3 1999-03-11 17:38:59 burghart Exp $";

# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/Label.h>
# include <X11/Xaw/Paned.h>
# include <X11/Xaw/Command.h>


# include <defs.h>
# include <message.h>
# include "HouseKeeping.h"
# include "radar_ingest.h"
# include "RadarInfo.h"


static zbool SWMade = FALSE;
static Widget AzLabel, ElLabel, FixedLabel, TimeLabel, GateLabel, ModeLabel;
static Widget BeamLabel, MissedLabel;
static Widget ThrEnable, ThrCountLbl;

static int UpdateSocket;
/*
 * Local variables mirroring the thresholding state, which we keep until
 * asked.
 */
static zbool LDoThresholding;
static unsigned char LThrCounts;

# ifdef __STDC__
	static Widget MakeStatusWidget (int, Widget, XtAppContext);
	static Widget MakeLabel (char *, Widget, Widget, Widget, char *,
			int, int);
	static void StatsWindow (Widget);
	static void ThreshWindow (Widget);
	static void ChangeEnable (void);
	static void ChangeCounts (Widget, XtPointer, XtPointer);
# else
	static Widget MakeStatusWidget ();
	static Widget MakeLabel ();
	static void StatsWindow ();
	static void ThreshWindow ();
	static void ChangeEnable ();
	static void ChangeCounts ();
# endif


static int
IgnoreRinfo ()
{ 
	return (0);
}



DefineWidgets ()
/*
 * Make our widgets known to the system.
 */
{
	uw_def_widget ("status", "Rasterizer status", MakeStatusWidget, 0, 0);
/*
 * Kludge: hardwired net for now.
 */
	UpdateSocket = msg_BCSetup (0x8075d200, RadarInfoPort, IgnoreRinfo);
	msg_ELog (EF_INFO, "BCasting on skt %d, port %d", UpdateSocket,
			RadarInfoPort);
}





static Widget
MakeStatusWidget (junk, parent, appc)
int junk;
Widget parent;
XtAppContext appc;
/*
 * Create the status widget.
 */
{
	Widget pain;
	Arg args[5];
	int n;

	SWMade = TRUE;
/* 
 * At the top is a Paned widget to hold the pieces together.
 */
	n = 0;
	XtSetArg (args[n], XtNx, 800);	n++;
	XtSetArg (args[n], XtNy, 800);	n++;
	pain = XtCreateManagedWidget ("Pain", panedWidgetClass, parent,args,n);
/*
 * Add the rasterization stats window.
 */
	StatsWindow (pain);
/*
 * Then the thresholding status window.
 */
	ThreshWindow (pain);
	return (pain);
}




static void
StatsWindow (parent)
Widget parent;
{
	Widget form, w, above, ww;
	Arg args[10];
	int n;
/*
 * Create the usual form to hold all this stuff.
 */
	n = 0;
	XtSetArg (args[n], XtNdefaultDistance, 5);	n++;
	XtSetArg (args[n], XtNborderWidth, 0);		n++;
	form = XtCreateManagedWidget ("statusform", formWidgetClass, parent,
		args, n);
/*
 * Most of the rest is done with labels.
 */
	above = MakeLabel ("LastBeam", form, NULL, NULL, "Last beam: ", 0, 0);
	TimeLabel = MakeLabel ("Time", form, above, NULL, "--", 80, 1);

	ww = w = MakeLabel ("Az", form, NULL, above, "Az: ", 0, 0);
	AzLabel = MakeLabel ("AzLabel", form, w, above, "--", 60, 1);

	w = MakeLabel ("El", form, AzLabel, above, "El: ", 0, 0);
	ElLabel = MakeLabel ("ElLabel", form, w, above, "--", 60, 1);

	w = MakeLabel ("Fixed", form, ElLabel, above, "Fixed: ", 0, 0);
	FixedLabel = MakeLabel ("FxLabel", form, w, above, "--", 60, 1);

	above = ww;
	ww = MakeLabel ("Beams", form, NULL, above, "Beams read: ", 0, 0);
	BeamLabel = MakeLabel ("BLabel", form, ww, above, "--", 60, 1);
	w = MakeLabel ("Missed", form, BeamLabel, above,"Packets missed:",0,0);
	MissedLabel = MakeLabel ("MLabel", form, w, above, "--", 50, 1);
}




static void
ThreshWindow (parent)
Widget parent;
/*
 * Create the thresholding window.
 */
{
	Widget form, w;
	Arg args[5];
	int n;
	char count[20];
/*
 * Create the usual form to hold all this stuff.
 */
	n = 0;
	XtSetArg (args[n], XtNdefaultDistance, 5);	n++;
	XtSetArg (args[n], XtNborderWidth, 0);		n++;
	form = XtCreateManagedWidget ("statusform", formWidgetClass, parent,
		args, n);
/*
 * The basic label.
 */
	w = MakeLabel ("ThrTitle", form, NULL, NULL, "Thresholding:", 0, 0);
/*
 * Now we need a command widget to turn it on or off.
 */
	n = 0;
	XtSetArg (args[n], XtNlabel, DoThresholding ? "Enabled" : "Disabled");
							n++;
	XtSetArg (args[n], XtNfromHoriz, w);		n++;
	XtSetArg (args[n], XtNfromVert, NULL);		n++;
	ThrEnable = XtCreateManagedWidget ("ThrEnable", commandWidgetClass,
			form, args, n);
	XtAddCallback(ThrEnable, XtNcallback, (XtCallbackProc)ChangeEnable, 0);
/*
 * Labels for the counts.
 */
	w = MakeLabel ("ThrCountTitle", form, ThrEnable, NULL, "Counts:",0, 0);
	sprintf (count, "%d", ThrCounts);
	ThrCountLbl = MakeLabel ("ThrCounts", form, w, NULL, count, 40, 1);
/*
 * A couple of command widgets to change the counts.
 */
	n = 0;
	XtSetArg (args[n], XtNlabel, "\335");		n++;
	XtSetArg (args[n], XtNfromHoriz, ThrCountLbl);	n++;
	XtSetArg (args[n], XtNfromVert, NULL);		n++;
	w = XtCreateManagedWidget ("CountUp", commandWidgetClass, form,args,n);
	XtAddCallback (w, XtNcallback, (XtCallbackProc)ChangeCounts, 
		       (XtPointer)1);
	n = 0;
	XtSetArg (args[n], XtNlabel, "\337");		n++;
	XtSetArg (args[n], XtNfromHoriz, w);		n++;
	XtSetArg (args[n], XtNfromVert, NULL);		n++;
	w = XtCreateManagedWidget ("CountDown",commandWidgetClass,form,args,n);
	XtAddCallback (w, XtNcallback, ChangeCounts, 0);
/*
 * Save the current thresholding values, and we're done.
 */
	LDoThresholding = DoThresholding;
	LThrCounts = ThrCounts;
}





static void
ChangeEnable ()
/*
 * Toggle the thresholding state.
 */
{
	Arg args[2];

	LDoThresholding = ! LDoThresholding;
	XtSetArg (args[0], XtNlabel, LDoThresholding ? "Enabled" : "Disabled");
	XtSetValues (ThrEnable, args, 1);
}





static void
ChangeCounts (w, up, junk)
Widget w;
XtPointer up, junk;
/*
 * Change the number of counts used in thresholding.
 */
{
	char string[20];
	Arg args[2];
/*
 * Adjust the count.
 */
	if (up)
	{
		if (LThrCounts < 200)	/* Oughtta be enough */
			LThrCounts++;
	}
	else
		if (LThrCounts > 0)
			LThrCounts--;
/*
 * Then set the new value into the widget.
 */
	sprintf (string, "%d", LThrCounts);
	XtSetArg (args[0], XtNlabel, string);
	XtSetValues (ThrCountLbl, args, 1);
}




void
UpdateThreshold ()
/*
 * Load the thresholding parameters into the active variables.
 */
{
	DoThresholding = LDoThresholding;
	ThrCounts = LThrCounts;
}





static Widget
MakeLabel (name, parent, horiz, vert, label, width, border)
char *name, *label;
Widget parent, horiz, vert;
int width, border;
/*
 * Create a label widget.
 */
{
	int n;
	Arg args[10];

	n = 0;
	XtSetArg (args[n], XtNlabel, label);		n++;
	XtSetArg (args[n], XtNfromHoriz, horiz);	n++;
	XtSetArg (args[n], XtNfromVert, vert);		n++;
	XtSetArg (args[n], XtNborderWidth, border);	n++;
	if (width > 0)
	{
		XtSetArg (args[n], XtNwidth, width);	n++;
	}
	return (XtCreateManagedWidget (name, labelWidgetClass, parent,args,n));
}




static void
SetLabel (w, label)
Widget w;
char *label;
/*
 * Set this label.
 */
{
	Arg args[2];

	XtSetArg (args[0], XtNlabel, label);
	XtSetValues (w, args, 1);
}



void
SetStatus (hk)
Housekeeping *hk;
/*
 * Set the status widget.
 */
{
	char string[200];
	Arg args[5];
	RadarInfo update;
/*
 * Send out an update packet.
 */
	update.ri_last.ds_yymmdd = hk->year*10000 + hk->month*100 + hk->day;
	update.ri_last.ds_hhmmss = hk->hour*10000 + hk->minute*100 +hk->second;
	update.ri_az = hk->azimuth/CORR_FACT;
	update.ri_el = hk->elevation/CORR_FACT;
	update.ri_fixed = hk->fixed/CORR_FACT;
	update.ri_mode = hk->scan_mode;
	msg_BCast (UpdateSocket, &update, sizeof (update));

	if (! SWMade)
		return;
/*
 * Update the widget.
 */
	sprintf (string, "%d:%02d:%02d", hk->hour, hk->minute, hk->second);
	SetLabel (TimeLabel, string);

	sprintf (string, "%.1f", hk->azimuth/CORR_FACT);
	SetLabel (AzLabel, string);
	sprintf (string, "%.1f", hk->elevation/CORR_FACT);
	SetLabel (ElLabel, string);
	sprintf (string, "%.1f", hk->fixed/CORR_FACT);
	SetLabel (FixedLabel, string);

	sprintf (string, "%d", NBeam);
	SetLabel (BeamLabel, string);
	sprintf (string, "%d", NMissed);
	SetLabel (MissedLabel, string);
	uw_xevent ();
}
