/*
 * Quick and dirty radar status display.
 */
# include <X11/Intrinsic.h>
# include <X11/Xaw/Form.h>
# include <X11/Xaw/Command.h>
# include <X11/Shell.h>
# include <X11/StringDefs.h>
# include <X11/Xaw/Label.h>
# include <X11/Xaw/Cardinals.h>
# include "defs.h"
# include "../include/message.h"
# include "../include/dm.h"
# include "../include/timer.h"
# include "RadarInfo.h"

static char *rcsid = "$Id: RadarStatus.c,v 2.0 1991-07-18 23:18:20 corbet Exp $";


/*
 * Our widgets.
 */
Widget Top, Second, Shell, Form, Wm, DateLabel;
XtAppContext Appc;
bool Visible = FALSE;
bool Override = TRUE;

bool DataSeen = FALSE;

# ifdef __STDC__
	static int	GetStatus (int, char *, int);
	static void	MsgInput (XtPointer, int *, XtInputId *);
	static void	MsgBCInput (XtPointer, int *, XtInputId *);
	static void	Timeout (void);
# else
	static int	GetStatus ();
	static void	MsgInput ();
	static void	MsgBCInput ();
	static void	Timeout ();
# endif


main (argc, argv)
int argc;
char **argv;
{
	Arg args[20];
	Widget w, label;
	int xevent (), msg_event (), clearbutton (), wm (), n, bcs;
/*
 * Hook into the message system.
 */
	if (! msg_connect (msg_event, "RadarStatus"))
	{
		printf ("Unable to connect to message handler\n");
		exit (1);
	}
	if ((bcs = msg_BCSetup (0, RadarInfoPort, GetStatus)) < 0)
	{
		msg_ELog (EF_EMERGENCY, "Unable to get radar status port");
		exit (1);
	}
/*
 * Get set up with the toolkit.
 */
	Top = XtAppInitialize (&Appc, "RadarStatus", NULL, 0, &argc, argv,
		NULL, NULL, 0);
/*
 * Create our shell.
 */
	n = 0;
	XtSetArg (args[n], XtNinput, True);	n++;
	XtSetArg (args[n], XtNoverrideRedirect, True);	n++;
	Shell = XtCreatePopupShell ("RadarStatus", topLevelShellWidgetClass,
		Top, args, n);
/*
 * Put a form inside it.
 */
	Form = XtCreateManagedWidget ("form", formWidgetClass, Shell, args, 0);
/*
 * The label.
 */
	n = 0;
	XtSetArg (args[n], XtNfromHoriz, NULL);		n++;
	XtSetArg (args[n], XtNfromVert, NULL);		n++;
	XtSetArg (args[n], XtNlabel, "CP4 radar: --:--:--");	n++;
	XtSetArg (args[n], XtNborderWidth, 0);		n++;
	DateLabel = label = XtCreateManagedWidget ("datelabel", labelWidgetClass,
			Form, args, n);
/*
 * The window manager button.
 */
	n = 0;	
	XtSetArg (args[n], XtNfromHoriz, label);	n++;
	XtSetArg (args[n], XtNfromVert, NULL);		n++;
	XtSetArg (args[n], XtNlabel, "Ctl: DM");	n++;
	w = Wm = XtCreateManagedWidget("wm",commandWidgetClass, Form, args, n);
	XtAddCallback (Wm, XtNcallback, wm, 0);
/*
 * The second line line label.
 */
	n = 0;
	XtSetArg (args[n], XtNfromHoriz, NULL);			n++;
	XtSetArg (args[n], XtNfromVert, label);			n++;
	XtSetArg (args[n], XtNlabel, "Radar offline");		n++;
	XtSetArg (args[n], XtNborderWidth, 0);		n++;
	Second = XtCreateManagedWidget ("second", labelWidgetClass,
			Form, args, n);
# ifdef notdef
/*
 * Tell msglib about our X connection.
 */
	msg_add_fd (XConnectionNumber (XtDisplay (Shell)), xevent);
# endif
/*
 * Instead, tell X about the msg connection.
 */
	XtAppAddInput (Appc, msg_get_fd (), XtInputReadMask, MsgInput, 0);
	XtAppAddInput (Appc, bcs, XtInputReadMask, MsgBCInput, 0);
/*
 * A timer event to turn back to "offline" if data drops out.
 */
	tl_AddRelativeEvent (Timeout, 0, 30*INCFRAC, 30*INCFRAC);
/*
 * Now we just wait.  If we were given a command line argument, assume
 * they want to see us now; otherwise we await something from DM.
 */
	if (argc > 1)
	{
		Visible = Override = TRUE;
		wm ();
	}
	XtAppMainLoop (Appc);
# ifdef notdef
	sync ();
	xevent ();
	msg_await ();
# endif
}




static void
MsgInput (junk, fd, morejunk)
XtPointer junk;
int *fd;
XtInputId *morejunk;
/*
 * Input is happening.
 */
{
	if (msg_incoming (*fd))
		exit (1);
}





static void
MsgBCInput (junk, fd, morejunk)
XtPointer junk;
int *fd;
XtInputId *morejunk;
/*
 * Input is happening.
 */
{
	msg_BCHandler (*fd);
}








xevent (fd)
int fd;
/*
 * Deal with an Xt event.
 */
{
	XEvent event;
/*
 * Deal with events as long as they keep coming.
 */
 	while (XtAppPending (Appc))
	{
		XtAppNextEvent (Appc, &event);
		XtDispatchEvent (&event);
	}
	return (0);
}






msg_event (msg)
struct message *msg;
/*
 * Log a client event.
 */
{
/*
 * Maybe it's a display manager message.
 */
	if (msg->m_proto == MT_DISPLAYMGR)
		dm_msg (msg->m_data);
/*
 * Everything else is assumed to be a message handler event.
 */	
	else if (msg->m_proto == MT_MESSAGE)
	{
		struct mh_template *tmpl = (struct mh_template *) msg->m_data;
		if (tmpl->mh_type == MH_SHUTDOWN)
			exit (0);
	}
	return (0);
}








sync ()
/*
 * Synchronize with the window system.
 */
{
	XSync (XtDisplay (Top), False);
}






dm_msg (dmsg)
struct dm_msg *dmsg;
/*
 * Deal with a DM message.
 */
{
	struct dm_hello reply;
/*
 * See what we got.
 */
	switch (dmsg->dmm_type)
	{
	/*
	 * Maybe it's a DM scoping us out.
	 */
	   case DM_HELLO:
	   	reply.dmm_type = DM_HELLO;
		msg_send ("Displaymgr", MT_DISPLAYMGR, FALSE, &reply, 
			sizeof (reply));
		break;
	/*
	 * Maybe it's a reconfig.
	 */
	   case DM_RECONFIG:
		if (Override)
		   	reconfig (dmsg->dmm_x, dmsg->dmm_y, dmsg->dmm_dx,
				dmsg->dmm_dy);
		break;
	/*
	 * They might want us to go away entirely.
	 */
	   case DM_SUSPEND:
	   	if (Visible)
		{
			Visible = FALSE;
			XtPopdown (Shell);
		}
		break;
	}
}







reconfig (x, y, w, h)
int x, y, w, h;
/*
 * Reconfigure the window.
 */
{
	Arg args[5];

	XtSetArg (args[0], XtNx, x);
	XtSetArg (args[1], XtNy, y);
	XtSetArg (args[2], XtNwidth, w);
	XtSetArg (args[3], XtNheight, h);
	XtSetValues (Shell, args, 4);
/* 
 * If they can't see us yet, make it so now.
 */
	if (! Visible)
	{
		XtPopup (Shell, XtGrabNone);
		Visible = TRUE;
	}
	sync ();
}






wm ()
/*
 * Try to change override redirect.
 */
{
	Arg args[2];
/*
 * If the window is up, take it down.
 */
	if (Visible)
		XtPopdown (Shell);
/*
 * Set the parameter.
 */
	Override = ! Override;
	XtSetArg (args[0], XtNoverrideRedirect, Override);
	XtSetValues (Shell, args, 1);
/*
 * Set the label on the command widget too.
 */
	if (Override)
		XtSetArg (args[0], XtNlabel, "Ctl: DM");
	else
		XtSetArg (args[0], XtNlabel, "Ctl: WM");
	XtSetValues (Wm, args, 1);
/*
 * Put the window back if it was before.
 */
	if (Visible)
		XtPopup (Shell, XtGrabNone);
}





GetStatus (port, data, len)
int port, len;
char *data;
/*
 * Deal with a broadcast message.
 */
{
	RadarInfo *info = (RadarInfo *) data;
	char string[100];
	Arg args[2];
	static char *modes[] = { "CAL", "PPI", "COP", "RHI",
				 "??4", "??5", "??6", "??7", "SUR" };

	sprintf (string, "CP4 radar: %2d:%02d:%02d",
		info->ri_last.ds_hhmmss/10000,
		(info->ri_last.ds_hhmmss/100) % 100,
		info->ri_last.ds_hhmmss % 100);
	XtSetArg (args[0], XtNlabel, string);
	XtSetValues (DateLabel, args, 1);

	sprintf (string, "Az: %5.1f, El: %5.1f, Fixed: %5.1f, Mode: %s",
		info->ri_az, info->ri_el, info->ri_fixed, 
		modes[info->ri_mode]);
	XtSetArg (args[0], XtNlabel, string);
	XtSetValues (Second, args, 1);

	DataSeen = TRUE;
	sync ();
}





static void
Timeout ()
/*
 * See if there's been data.
 */
{
	Arg args[2];

	if (! DataSeen)
	{
		XtSetArg (args[0], XtNlabel, "-- Radar offline --");
		XtSetValues (Second, args, 1);
	}
	DataSeen = FALSE;
}
