/*
 * Widget for getting position of cursor.
 */
static char *rcsid = "$Id: PositionWidget.c,v 1.3 1991-10-15 22:23:21 kris Exp $";
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
# include <math.h>

# include <X11/Intrinsic.h>
# include <X11/StringDefs.h>
# include <X11/Shell.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/Label.h>
# include <X11/Xaw/Command.h>
# include <X11/Xaw/AsciiText.h>

# include "../include/defs.h"
# include "../include/message.h"
# include "../include/pd.h"
# include "PixelCoord.h"
# include "GraphProc.h"

# define PI 3.141592654
# define MAXORG 20

static Widget 	PosLabel = NULL, DMSButton, OrgButton, OrgLabel;
static Widget	KNButton;
static int 	PWMade = FALSE, DegMinSec = TRUE, DoNm = TRUE;
static char 	Origins[MAXORG*20], *ONames[MAXORG];
static int 	NOrg, OIndex = 0;

# ifdef __STDC__
	static void pw_PosPopup ();
	static void pw_PosPopdown (Widget, int, int);
	void pw_PosStatus ();
	void pw_InitPos();
	Widget pw_PosCreate (char *, Widget, XtAppContext);
	void ChangeType ();
	void ChangeUnit ();
	static void ChangeOrg (Widget, XtPointer, XtPointer);
# endif


void
pw_PosPopdown (w, junk1, junk2)
Widget 	w; 
int 	junk1, junk2;
/*
 * Pop down this widget.
 */
{
	uw_popdown ("position");
}


void
pw_InitPos ()
/*
 * Tell UI about the position widget.
 */
{
	uw_def_widget ("position", "Get Position", pw_PosCreate, 0, 0);
}


Widget
pw_PosCreate (junk, parent, actx)
char 	*junk;
Widget 	parent;
XtAppContext 	actx;
/*
 * Actually create the position widget.
 */
{
	Widget	form;
	Arg	args[10];
	int	n;

/*
 * Make the bitmaps for the left and right arrow buttons.
 */
	bm_BuildBitmaps (parent);
/*
 * Find out what the possible origins are.
 */
	if (! pda_Search (Pd, "global", "pos-origin", NULL, Origins, 
		SYMT_STRING))
	{
		msg_ELog (EF_PROBLEM, "No origin list.");
		return;
	}
	NOrg = CommaParse (Origins, ONames);
/*
 * The text window which displays position.
 */
	n = 0;
	XtSetArg (args[n], XtNlabel, "");		n++;
	XtSetArg (args[n], XtNjustify, XtJustifyLeft);	n++;
	XtSetArg (args[n], XtNresize, True);		n++;
	XtSetArg (args[n], XtNwidth, 300);		n++;
	XtSetArg (args[n], XtNheight, 75);		n++;
	PosLabel = XtCreateManagedWidget ("GetPosition", labelWidgetClass,
		parent, args, n);
/*
 * The button to switch between deg/min/sec and decimal degrees
 */
	n = 0;
	XtSetArg (args[n], XtNlabel, DegMinSec ? "Deg/Min/Sec" : 
		"Decimal Deg"); n++;
	XtSetArg (args[n], XtNfromHoriz, NULL);			n++;
	XtSetArg (args[n], XtNfromVert, PosLabel);		n++;
	DMSButton = XtCreateManagedWidget ("degminsec", commandWidgetClass,
		parent, args, n);
	XtAddCallback (DMSButton, XtNcallback, ChangeType, 0);
/*
 * The button to switch between km and nm.
 */
	n = 0;
	XtSetArg (args[n], XtNlabel, DoNm ? "Nm" : "km");	n++;
	XtSetArg (args[n], XtNfromHoriz, NULL);			n++;
	XtSetArg (args[n], XtNfromVert, DMSButton);		n++;
	KNButton = XtCreateManagedWidget ("nmkm", commandWidgetClass,
		parent, args, n);
	XtAddCallback (KNButton, XtNcallback, ChangeUnit, 0);
/*
 * The button to switch origins.
 */
	OrgButton = LeftRightButtons (parent, ChangeOrg, NULL);

	n = 0;
	XtSetArg (args[n], XtNfromHoriz, DMSButton);	n++;
	XtSetArg (args[n], XtNfromVert, PosLabel);	n++;
	XtSetValues (OrgButton, args, n);
/*
 * The origin label.
 */
	n = 0;
	XtSetArg (args[n], XtNborderWidth, 0);		n++;
	XtSetArg (args[n], XtNlabel, ONames[OIndex]);	n++;
	XtSetArg (args[n], XtNfromHoriz, OrgButton);	n++;
	XtSetArg (args[n], XtNfromVert, PosLabel);	n++;
	OrgLabel = XtCreateManagedWidget ("orglabel", labelWidgetClass,
		parent, args, n);

	PWMade = TRUE;
	return (PosLabel);
}


static void
ChangeOrg (w, change, junk)
Widget 		w;
XtPointer	change, junk;
/*
 * Change the origin for a range azimuth display.
 */
{
	Arg	args[2];
	int	delta = (int) change;

	OIndex = (OIndex + delta) % NOrg;
	if (OIndex < 0)
		OIndex += NOrg;
	XtSetArg (args[0], XtNlabel, ONames[OIndex]);
	XtSetValues (OrgLabel, &args, 1);
}


void
ChangeType ()
/*
 * Change the type of the position display between Lat/Lon and Range/Azimuth.
 */
{
	Arg	args[2];

	DegMinSec = ! DegMinSec;
	XtSetArg (args[0], XtNlabel, DegMinSec ? "Deg/Min/Sec" : 
		"Decimal Deg");
	XtSetValues (DMSButton, args, 1);
}


void
ChangeUnit ()
/*
 * Change the position display between km and nm.
 */
{
	Arg	args[2];

	DoNm = ! DoNm;
	XtSetArg (args[0], XtNlabel, DoNm ? "Nm" : "km");
	XtSetValues (KNButton, args, 1);
}


void
pw_PosStatus ()
/*
 * Create the position text line and display it in the widget.
 */
{
	char	string[100], label[300], offstring[50];
	Window	wjunk;
	int	junk, x, y;
	float	offset, lat, lon, ox, oy; 
	double	range, azimuth, subx, suby;
	Location	loc;
	Arg	args[5];

	if (! PWMade) return;
/*
 * Empty the label
 */
	strcpy (label, "");
/*
 * Find out where the pointer is.
 */
	XQueryPointer (XtDisplay (Graphics), XtWindow (Graphics), &wjunk, 
		&wjunk, &junk, &junk, &x, &y, (unsigned int *) &junk);
/*
 * Calculate the lat/lon
 */
	cvt_ToLatLon (XUSER(x), YUSER(y), &lat, &lon);
	if (DegMinSec)
	{
		int	latdeg, latmin, latsec, londeg, lonmin, lonsec;
		char	ewdir, nsdir;

		ewdir = (lon > 0) ? 'E' : 'W';
		nsdir = (lat > 0) ? 'N' : 'S';

		lat = fabs (lat);
		lon = fabs (lon);

		latdeg = (int) lat;
		latmin = (int)((lat - latdeg) * 60);
		latsec = (int)((lat - latdeg - latmin / 60.0) * 3600);

		londeg = (int) lon;
		lonmin = (int)((lon - londeg) * 60);
		lonsec = (int)((lon - londeg - lonmin / 60.0) * 3600);

		sprintf (string, "Lat: %d %d'%d'' %c  Lon: %d %d'%d'' %c\n\n", 
			latdeg, latmin, latsec, nsdir, londeg, lonmin, 
			lonsec, ewdir);
	}
	else
		sprintf (string, "Lat: %.2f Lon: %.2f\n\n", lat, lon);

	strcat (label, string);
/*
 * Get the location and the azimuth offset of the origin.
 */
	if (! GetLocation (ONames[OIndex], &PlotTime, &loc))
	{
		msg_ELog (EF_PROBLEM, "Unable to locate origin %s.",
			ONames[OIndex]);
		return;
	}
	sprintf (offstring, "%s-azimuth-offset", ONames[OIndex]);
	if (! pda_Search (Pd, "global", offstring, NULL, 
		(char *) &offset, SYMT_FLOAT))
		offset = 0.0;			
/*
 * Calculate range and azimuth
 */
	cvt_ToXY (loc.l_lat, loc.l_lon, &ox, &oy);
	subx = (double) (XUSER(x) - ox);
	suby = (double) (YUSER(y) - oy);

	range = hypot (subx, suby);
	if (DoNm)
		range = range / 6080.0 * (5280.0 * 0.621);

	azimuth =  atan2 (suby, subx);
	azimuth = 90.0 - (azimuth * 180.0 / PI) + offset;
	if (azimuth < 0.0)
		azimuth += 360.0;

	sprintf (string, "Az: %.0f  Range: %.0f %s\n", azimuth, range, 
		DoNm ? "Nm" : "km");
	strcat (label, string);
	sprintf (string, "Relative to %s", ONames[OIndex]);
	strcat (label, string);

	XtSetArg (args[0], XtNlabel, label);
	XtSetValues (PosLabel, args, 1);
}
