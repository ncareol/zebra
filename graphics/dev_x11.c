/* 12/88 jc */
/* $Id: dev_x11.c,v 1.12 1989-10-11 15:46:16 corbet Exp $	*/
/*
 * Graphics driver for the X window system, version 11.3
 */
# include "config.h"

# ifdef DEV_X11

# include "graphics.h"
# include "device.h"
# include <stdio.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/cursorfont.h>
# include <X11/keysym.h>

char *getvm ();

# define TGT_SIZE	17

# define ABS(x)		((x) < 0 ? -(x) : x)

# define TRUE	1
# define FALSE	0

typedef struct {
	short x, y;
} Xpoint;

/*
 * The device structure.
 */
struct xtag
{
	Display	*x_display;	/* The display structure	*/
	Window 	x_window;	/* The window.			*/
	Pixmap  x_sw[5];	/* Quadrant subwindows		*/
	GC	x_gc;		/* Graphics context		*/
	Cursor	x_cursor;	/* The cursor for this window	*/
	Visual	*x_visual;	/* The visual			*/
	int	x_xres, x_yres;	/* The resolution of our window	*/
	int	x_mono;		/* Is this a mono screen?	*/
	long	x_fg, x_bg;	/* Foreground and background colors	*/
	int 	*x_dev_bg;	/* KLUGE: pointer to bg color in dev struct */
	int	x_xtgt, x_ytgt;	/* Target position			*/
	GC	x_tgt_gc;	/* Target GC (no clipping)		*/
	int	x_do_pxm;	/* Do we need to do the target pixmap?	*/
	Pixmap	x_tgt_pixmap;	/* Pixmap to save data covered by target*/
	char	*x_zwork;	/* Zoom workspace			*/
	int	x_zx, x_zy;	/* Zoom origins				*/
	char	x_zoomok;	/* Zoom is up to date.			*/
	char	x_zquad;	/* Which quad we are zoomed on		*/
	char	x_qdone[5];	/* Is this quad done?			*/
};


/*
 * Dash patterns.
 */
static char Dp_dash[2] = { 8, 8 };
static char Dp_dot[2] = { 2, 4 };
static char Dp_dash_dot[4] = { 8, 4, 2, 4 };

static struct 
{
	char	*d_pattern;
	int	d_ndash;
} D_table[4] =
{
	{ 0, 0 },
	{ Dp_dash,	2 },
	{ Dp_dot,	2 },
	{ Dp_dash_dot,	4 }
};


/*
 * The table used for zooming.
 */
static unsigned short Ztable[256];



x11_open (device, type, ctag, dev)
char *device, *type, **ctag;
struct device *dev;
/*
 * Open up an X11 window device.
 */
{
	struct xtag *tag = (struct xtag *) getvm (sizeof (struct xtag));
	XSetWindowAttributes attr;
	XGCValues gcontext;
	XVisualInfo template, *vlist;
	int screen, pv[2], pm, nmatch, depth, i;
	XEvent ev;
/*
 * Figure out what our resolution will be.
 */
 	if (! strcmp (type, "X11") || ! strcmp (type, "X700") ||
		! strcmp (type, "x11") || ! strcmp (type, "x700"))
		tag->x_xres = tag->x_yres = 700;
	else if (! strcmp (type, "X11-huge") || ! strcmp (type, "x11-huge"))
	{
		tag->x_xres = 900;
		tag->x_yres = 800;
	}
	else
		tag->x_xres = tag->x_yres = 500;
/*
 * First, open up our display.  If the device is "screen", convert it to
 * unix:0; otherwise take the device as given.
 */
	tag->x_display = XOpenDisplay (device && strcmp (device, "screen") ? 
			device : "unix:0.0");
	if (! tag->x_display)
	{
		relvm (tag);
		return (GE_BAD_DEVICE);
	}
	screen = DefaultScreen (tag->x_display);
	tag->x_mono = DefaultDepth (tag->x_display, screen) == 1;

	tag->x_fg = 1;
	tag->x_bg = 0;
/*
 * No target for now
 */
	tag->x_xtgt = -1;
	tag->x_ytgt = -1;
/*
 * Try to find a pseudocolor visual.  If we succeed, we use it; otherwise
 * it's monochrome city.  We really won't work well with anything but a
 * 8 bit visual, but if we ask for that the Ardent server freaks.
 */
	template.screen = screen;
	template.class = PseudoColor;
	vlist = XGetVisualInfo (tag->x_display,
		VisualScreenMask | VisualClassMask, &template, &nmatch);
	printf ("We have %d visual matches\n", nmatch);
	tag->x_mono = ! nmatch;
	if (nmatch)
	{
		tag->x_visual = vlist->visual;
		depth = vlist->depth;
	}
	else
	{
		tag->x_visual = DefaultVisual (tag->x_display, screen);
		depth = CopyFromParent;
	}
	printf ("Visual is 0x%x depth %d\n", tag->x_visual, vlist->depth);
 	XFree (vlist);
/*
 * Create the window to exist on that display.
 */
	attr.background_pixel = BlackPixel (tag->x_display, screen);
	attr.border_pixel = WhitePixel (tag->x_display, screen);
	attr.backing_store = WhenMapped;
	attr.event_mask = ButtonPressMask | ExposureMask | KeyPressMask;
	tag->x_window = XCreateWindow (tag->x_display,
		RootWindow (tag->x_display, screen), 10, 10, tag->x_xres, 
		tag->x_yres, 2, depth, InputOutput, tag->x_visual,
		CWBackPixel|CWBorderPixel|CWBackingStore|CWEventMask, &attr);
/*
 * Create all of the subwindows.
 */
	if (! tag->x_mono)
		for (i = 0; i < 5; i++)
			tag->x_sw[i] = XCreatePixmap (tag->x_display,
				tag->x_window, tag->x_xres, tag->x_yres, 8);
/*
 * Store some properties.
 */
	XStoreName (tag->x_display, tag->x_window, "RDSS Graphics");
	XSetIconName (tag->x_display, tag->x_window, "Graphics");
/*
 * Map the window.  (Sync it to get the wmgr mapping process underway.)
 */
 	XMapWindow (tag->x_display, tag->x_window);
/*
 * Get the pixmap to save the area covered by the target
 */
	tag->x_tgt_pixmap = XCreatePixmap (tag->x_display, tag->x_window, 
		TGT_SIZE + 2, TGT_SIZE + 2,
		DefaultDepth (tag->x_display, screen));
/*
 * Get a graphics context.
 */
 	gcontext.foreground = WhitePixel (tag->x_display, screen);
	gcontext.background = BlackPixel (tag->x_display, screen);
	tag->x_gc = XCreateGC (tag->x_display, tag->x_window, 
		GCForeground | GCBackground, &gcontext);
/*
 * Create a separate graphics context for the target, since we don't want
 * clipping to affect it
 */
	tag->x_tgt_gc = XCreateGC (tag->x_display, tag->x_window, 
		GCForeground | GCBackground, &gcontext);
/*
 * Get the cursor set up.
 */
 	tag->x_cursor = XCreateFontCursor (tag->x_display, XC_top_left_arrow);
	XDefineCursor (tag->x_display, tag->x_window, tag->x_cursor);
/*
 * Fill in the device structure.
 */
 	if (tag->x_mono)
		dev->gd_ncolor = 2;
	else
	{
		dev->gd_ncolor = DisplayCells (tag->x_display, screen);
		dev->gd_flags |= GDF_PIXEL;
	}
	dev->gd_xres = tag->x_xres;
	dev->gd_yres = tag->x_yres;
	dev->gd_background = tag->x_bg;
/*
 * KLUGE: Keep a pointer to the background color in the dev structure, since
 * we need to change it when the user first allocates some colors
 */
	tag->x_dev_bg = &(dev->gd_background);
/*
 * Clear the window.
 */
	printf ("Waiting for expose..."); fflush (stdout);
	XWindowEvent (tag->x_display, tag->x_window, ExposureMask, &ev);
	XClearWindow (tag->x_display, tag->x_window);
	printf ("Done.\n");
/*
 * Initialize zoom stuff.
 */
	tag->x_zwork = 0;
	tag->x_zoomok = tag->x_zquad = 0;
	for (i = 0; i < 256; i++)
		Ztable[i] = i | (i << 8);
/*
 * All done.
 */
 	*ctag = (char *) tag;
	G_setfd (XConnectionNumber (tag->x_display));
	return (GE_OK);
}





x11_close (ctag)
char *ctag;
/*
 * Finish up.
 */
{
	struct xtag *tag = (struct xtag *) ctag;

	XCloseDisplay (tag->x_display);
	if (tag->x_zwork)
		relvm (tag->x_zwork);
	relvm (tag);
}





x11_clear (ctag)
char *ctag;
/*
 * Clear the window.
 */
{
	struct xtag *tag = (struct xtag *) ctag;

	XClearWindow (tag->x_display, tag->x_window);
	tag->x_zoomok = FALSE;
/*
 * Clear the target pixmap and remember we need to redo it
 */
	XSetForeground (tag->x_display, tag->x_tgt_gc, 
		BlackPixel (tag->x_display, 0));
	XFillRectangle (tag->x_display, tag->x_tgt_pixmap, tag->x_tgt_gc, 
		0, 0, TGT_SIZE + 2, TGT_SIZE + 2);
	XSetForeground (tag->x_display, tag->x_tgt_gc, 
		WhitePixel (tag->x_display, 0));

	tag->x_do_pxm = TRUE;
}




x11_poly (ctag, color, ltype, npt, data)
char *ctag;
int color, ltype, npt, *data;
/*
 * The polyline routine.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	Xpoint *xp;
	int pt;
/*
 * For color displays, fill in the color value.
 */
 	if (! tag->x_mono)
		XSetForeground (tag->x_display, tag->x_gc, color);
/*
 * Set up our dash pattern.
 */
	if (ltype == GPLT_SOLID)
		XSetLineAttributes (tag->x_display, tag->x_gc, 0, LineSolid,
			CapButt, JoinMiter);
	else
	{
		XSetLineAttributes (tag->x_display, tag->x_gc, 0,
			LineOnOffDash, CapButt, JoinMiter);
		XSetDashes (tag->x_display, tag->x_gc, 1,
			D_table[ltype].d_pattern, D_table[ltype].d_ndash);
	}
/*
 * Allocate our xpoint array, and fill it in.
 */
	xp = (Xpoint *) getvm (npt * sizeof (Xpoint));
	for (pt = 0; pt < npt; pt++)
	{
		xp[pt].x = *data++;
		xp[pt].y = tag->x_yres - (*data++ + 1);
	}
/*
 * Do the drawing.  If we are not zoomed, just dump it straight to the
 * screen; otherwise just draw into the pixmap.
 */
 	if (! tag->x_zquad)
	{
	 	XDrawLines (tag->x_display, tag->x_window, tag->x_gc, xp, npt,
			CoordModeOrigin);
		tag->x_zoomok = FALSE;
		relvm (xp);
		return;
	}
 	XDrawLines (tag->x_display, tag->x_sw[0], tag->x_gc, xp, npt,
		CoordModeOrigin);
/*
 * Go through and double each coordinate and draw into quad 1.
 */
	for (pt = 0; pt < npt; pt++)
	{
		xp[pt].x *= 2;
		xp[pt].y *= 2;
	}
 	XDrawLines (tag->x_display, tag->x_sw[1], tag->x_gc, xp, npt,
		CoordModeOrigin);
	if (tag->x_zquad == 1)
	 	XDrawLines (tag->x_display, tag->x_window, tag->x_gc, xp, npt,
			CoordModeOrigin);
/*
 * Offset into quadrant 2.
 */
 	for (pt = 0; pt < npt; pt++)
		xp[pt].x -= tag->x_xres;
 	XDrawLines (tag->x_display, tag->x_sw[2], tag->x_gc, xp, npt,
		CoordModeOrigin);
	if (tag->x_zquad == 2)
	 	XDrawLines (tag->x_display, tag->x_window, tag->x_gc, xp, npt,
			CoordModeOrigin);
/*
 * Offset into quadrant 4.
 */
 	for (pt = 0; pt < npt; pt++)
		xp[pt].y -= tag->x_yres;
 	XDrawLines (tag->x_display, tag->x_sw[4], tag->x_gc, xp, npt,
		CoordModeOrigin);
	if (tag->x_zquad == 4)
	 	XDrawLines (tag->x_display, tag->x_window, tag->x_gc, xp, npt,
			CoordModeOrigin);
/*
 * Offset into quadrant 3.
 */
	for (pt = 0; pt < npt; pt++)
		xp[pt].x += tag->x_xres;
 	XDrawLines (tag->x_display, tag->x_sw[3], tag->x_gc, xp, npt,
		CoordModeOrigin);
	if (tag->x_zquad == 3)
	 	XDrawLines (tag->x_display, tag->x_window, tag->x_gc, xp, npt,
			CoordModeOrigin);
	relvm (xp);
}






x11_flush (ctag)
char *ctag;
/*
 * Flush out any pending operations.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
/*
 * Draw in the target if it's been placed
 */
	if (tag->x_xtgt >= 0 && tag->x_ytgt >= 0)
	{
	/*
	 * Save the portion of the screen which will be overwritten by 
	 * the target, if necessary
	 */
		if (tag->x_do_pxm)
		{
			XCopyArea (tag->x_display, tag->x_window, 
				tag->x_tgt_pixmap, tag->x_tgt_gc, 
				tag->x_xtgt - TGT_SIZE/2 - 1, 
				tag->x_ytgt - TGT_SIZE/2 - 1, 
				TGT_SIZE + 2, TGT_SIZE + 2, 0, 0);
			tag->x_do_pxm = FALSE;
		}
	/*
	 * Draw the target
	 */
		XDrawArc (tag->x_display, tag->x_window, tag->x_tgt_gc, 
			tag->x_xtgt - TGT_SIZE/2, tag->x_ytgt - TGT_SIZE/2, 
			TGT_SIZE, TGT_SIZE, 0, 25000);
		XDrawLine (tag->x_display, tag->x_window, tag->x_tgt_gc, 
			tag->x_xtgt - TGT_SIZE/2, tag->x_ytgt, 
			tag->x_xtgt + TGT_SIZE/2, tag->x_ytgt);
		XDrawLine (tag->x_display, tag->x_window, tag->x_tgt_gc, 
			tag->x_xtgt, tag->x_ytgt - TGT_SIZE/2, 
			tag->x_xtgt, tag->x_ytgt + TGT_SIZE/2);
	}
/*
 * Flush everything out
 */
	XFlush (tag->x_display);
}





x11_noop ()
{
	return (GE_OK);
}





x11_pick (ctag, button, x, y)
char *ctag;
int *button, *x, *y;
/*
 * The target pick routine.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	XEvent ev;
/*
 * Move everything out the display, and clear the event queue, so that 
 * there is no grungy old stuff sitting around.
 */
 	XSync (tag->x_display, True);
/*
 * Now wait for an event.
 */
	for (;;)
	{
		XWindowEvent (tag->x_display, tag->x_window,
			ButtonPressMask | KeyPressMask, &ev);
		if (ev.type == KeyPress)
			x11_key (tag, &ev);
		else
			break;
	}		
	*button = ev.xbutton.button - 1;
	*x = ev.xbutton.x;
	*y = tag->x_yres - ev.xbutton.y - 1;
/*
 * If we are zoomed, we must adjust the coordinates accordingly.
 */
 	if (tag->x_zquad)
	{
		*x /= 2;
		*y /= 2;
	 	if (tag->x_zquad == 2 || tag->x_zquad == 4)
			*x += tag->x_xres/2;
		if (tag->x_zquad < 3)
			*y += tag->x_yres/2;
	}
}




x11_target (ctag, x, y)
char *ctag;
int *x, *y;
/*
 * Report the target location
 */
{
	struct xtag *tag = (struct xtag *) ctag;

	*x = tag->x_xtgt;
	*y = tag->x_yres - tag->x_ytgt;
	return;
}



x11_put_target (ctag, x, y)
char	*ctag;
int	x, y;
/*
 * Put the target at the chosen location
 */
{
	struct xtag *tag = (struct xtag *) ctag;
/*
 * Restore the data under the old target location (if there is an old
 * target location)
 */
	if (tag->x_xtgt > 0 && tag->x_ytgt > 0)
		XCopyArea (tag->x_display, tag->x_tgt_pixmap, tag->x_window,
			tag->x_tgt_gc, 0, 0, TGT_SIZE + 2, TGT_SIZE + 2, 
			tag->x_xtgt - TGT_SIZE/2 - 1, 
			tag->x_ytgt - TGT_SIZE/2 - 1);
/*
 * Save the new target location
 */
	tag->x_xtgt = x;
	tag->x_ytgt = tag->x_yres - y;
/*
 * Make sure we redo the pixmap on the next flush
 */
	tag->x_do_pxm = TRUE;

	x11_flush (ctag);
	return;
}




x11_untarget (ctag)
char	*ctag;
/*
 * Remove the target
 */
{
	struct xtag *tag = (struct xtag *) ctag;
/*
 * Restore the data under the old target location (if there is an old
 * target location)
 */
	if (tag->x_xtgt > 0 && tag->x_ytgt > 0)
		XCopyArea (tag->x_display, tag->x_tgt_pixmap, tag->x_window,
			tag->x_tgt_gc, 0, 0, TGT_SIZE + 2, TGT_SIZE + 2, 
			tag->x_xtgt - TGT_SIZE/2 - 1, 
			tag->x_ytgt - TGT_SIZE/2 - 1);
/*
 * Negative target location for no target
 */
	tag->x_xtgt = tag->x_ytgt = -1;

	x11_flush (ctag);
	return;
}




x11_casn (ctag, ncolor, base)
char *ctag;
int ncolor, *base;
/*
 * Grab some colors for the application.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	int	ncontig, nget, offset, ncells, contig_start;
	int	start, i, status;
	long	*cells, pm;
/*
 * Simple for a mono device
 */
	if (tag->x_mono)
	{
		*base = 0;
		return (GE_OK);
	}
/*
 * Start with space for 256 color cells (this may increase below)
 */
	ncells = 256;
	cells = (long *) malloc (ncells * sizeof (long));
/*
 * Initialize
 */
	offset = 0;
	ncontig = 1;
	nget = 0;
	contig_start = 0;
/*
 * Allocate color cells until we have the requested number of contiguous
 * cells
 */
	while (ncontig != ncolor)
	{
	/*
	 * Update the offset and the number of cells to get
	 */
		offset += nget;
		nget = ncolor - ncontig;
	/*
	 * Get more cell space if necessary
	 */
		if (offset + nget > ncells)
		{
			ncells *= 2;
			cells = (long *) 
				realloc (cells, ncells * sizeof (long));
		}
	/*
	 * Get as many color cells as we need to complete a chunk of ncolor
	 * contiguous cells
	 */
		status = XAllocColorCells (tag->x_display, 
			DefaultColormap (tag->x_display, 0), False, &pm, 0,
			cells + offset, nget);
		if (! status)
			return (GE_DEVICE_UNABLE);
	/*
	 * Find out how many contiguous cells we have now
	 */
		start = (offset == 0) ? 1 : offset;

		for (i = start; i < nget + offset; i++)
		{
			if (ABS (cells[i] - cells[i-1]) == 1)
				ncontig++;
			else
			{
				contig_start = i;
				ncontig == 1;
			}
		}
	}
/*
 * Deallocate the cells before the contiguous chunk
 */
	XFreeColors (tag->x_display, DefaultColormap (tag->x_display, 0), 
		cells, contig_start, 0);
/*
 * Save the base of the contiguous block
 */
	*base = cells[contig_start];
/*
 * Set the background if it hasn't been done
 */
	if (! tag->x_bg)
	{
		long	bg = cells[contig_start];

		tag->x_bg = bg;
		*(tag->x_dev_bg) = bg;
		XSetWindowBackground (tag->x_display, tag->x_window, bg);
	}
/*
 * Free the memory allocated for cells
 */
	free (cells);

	return (GE_OK);
}





x11_color (ctag, base, ncolor, r, g, b)
char *ctag;
int base, ncolor;
float *r, *g, *b;
{
	struct xtag *tag = (struct xtag *) ctag;
	XColor *xc = (XColor *) getvm (ncolor * sizeof (XColor));
	int col;
/*
 * Ignore this stuff for mono displays.
 */
 	if (tag->x_mono)
		return (GE_DEVICE_UNABLE);
/*
 * Reformat the colors for X.
 */
 	for (col = 0; col < ncolor; col++)
	{
		xc[col].pixel = col + base;
		xc[col].red = (unsigned short) (*r++ * 65535.0);
		xc[col].green = (unsigned short) (*g++ * 65535.0);
		xc[col].blue = (unsigned short) (*b++ * 65535.0);
		xc[col].flags = DoRed | DoGreen | DoBlue;
	}
/*
 * Now, store them.
 */
 	XStoreColors (tag->x_display, DefaultColormap (tag->x_display, 0), 
		xc, ncolor);
	relvm (xc);
		
	return (GE_OK);
}






x11_pixel (ctag, x, y, xs, ys, data, size, org)
char *ctag, *data;
int x, y, xs, ys, size, org;
/*
 * The pixel fill routine.
 */
{
	XImage *xi;
	struct xtag *tag = (struct xtag *) ctag;
# ifdef notdef
/*
 * Kludge to allow the restoration of images saved under Sunview.
 */
	if (xs > 300 || ys > 300)
		x11_sv_kludge (data, xs, ys, tag->x_bg);
# endif
	x11_zreset (tag);
/*
 * Create the XImage structure.
 */
 	xi = XCreateImage (tag->x_display, tag->x_visual, 8, ZPixmap, 0, data,
		xs, ys, 8, xs);
/*
 * Send it to the display.
 */
 	XPutImage (tag->x_display, tag->x_window, tag->x_gc, xi, 0, 0, 
		x, tag->x_yres - (y + ys), xs, ys);
/*
 * Release the image structure.
 */
 	xi->data = (char *) 0;
	XDestroyImage (xi);
}





x11_sv_kludge (data, xs, ys, base)
register unsigned char *data;
int xs, ys;
unsigned int base;
/*
 * Offset this data to the base needed for X, if necessary.
 */
{
	register int i, npix = xs*ys;
/*
 * Look at up to the first thousand pixels.  If any are below the nominal
 * base value, assume that we have to offset it.
 */
	for (i = 0; i < 1000; i++)
		if (data[i] < base)
			break;
	if (i >= 1000)
		return;
	if (getenv ("XBASE"))
		base = atoi (getenv ("XBASE"));
	printf ("Found %d at %d -- offsetting by %d\n", data[i], i, base);
/*
 * Do it.
 */
 	for (i = 0; i < npix; i++)
		data[i] = (data[i] == 127) ? 0 : data[i] + base;
}




x11_clip (ctag, x0, y0, x1, y1)
char	*ctag;
int	x0, y0, x1, y1;
/*
 * Set the "hardware" clip window
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	XRectangle	rect;

	rect.x = x0;
	rect.y = tag->x_yres - y1;
	rect.width = x1 - x0 + 1;
	rect.height = y1 - y0 + 1;	

	XSetClipRectangles (tag->x_display, tag->x_gc, 0, 0, &rect, 1, 
		Unsorted);
}



x11_zreset (tag)
struct xtag *tag;
/*
 * Reset our zoom.
 */
{
	if (tag->x_zquad)
	{
	 	XCopyArea (tag->x_display, tag->x_sw[0], tag->x_window,
			tag->x_tgt_gc, 0, 0, tag->x_xres, tag->x_yres, 0, 0);
		tag->x_zquad = 0;
	}
	tag->x_zoomok = FALSE;
}




x11_vp (ctag, x0, y0, x1, y1)
char *ctag;
int x0, y0, x1, y1;
/*
 * The change viewport routine -- only set up to do editor/perusal quads
 * for now.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	int whole;
/*
 * Mono devices don't do this for now.
 */
 	if (tag->x_mono)
		return (GE_DEVICE_UNABLE);
/*
 * See what sort of zoom is wanted.
 */
	whole = (x1 - x0)*3 > tag->x_xres*2 && (y1 - y0)*3 > tag->x_yres*2;
/*
 * Whole image.
 */
 	if (whole)
	{
		if (tag->x_zquad == 0)
			return (GE_OK);		/* Not zoomed	*/
	 	XCopyArea (tag->x_display, tag->x_sw[0], tag->x_window,
			tag->x_tgt_gc, 0, 0, tag->x_xres, tag->x_yres, 0, 0);
		tag->x_zquad = 0;
	}
/*
 * Quad.
 */
 	else
	{
	/*
	 * Calculate the quad.
	 */
		tag->x_zx = x0 ? tag->x_xres/2 : 0;
		tag->x_zy = y0 ? 0 : tag->x_yres/2;
		tag->x_zquad = (tag->x_zx ? 1 : 0) + (tag->x_zy ? 2 : 0) + 1;
	/*
	 * Create the zoomed image, if necessary.
	 */
		x11_calc_zoom (tag, tag->x_zquad);
	/*
	 * Now send the appropriate window.
	 */
	 	XCopyArea (tag->x_display, tag->x_sw[tag->x_zquad],
			tag->x_window, tag->x_tgt_gc, 0, 0, tag->x_xres,
			tag->x_yres, 0, 0);
	}
/*
 * Sync things out.
 */
 	x11_flush (ctag);
	return (GE_OK);
}




x11_do_quad (tag, x, y, q, read, write)
struct xtag *tag;
int x, y, q;
XImage *read, *write;
/*
 * Calculate a single quad.
 */
{
	register unsigned char *rp = (unsigned char *)
		tag->x_zwork + ((3*tag->x_xres)/4)*tag->x_yres;
	register short *wps = (short *) tag->x_zwork;
	register int row, col;
# ifdef __STDC__
	const int xwidth = tag->x_xres/2, xfull = tag->x_xres;
# else
	register int xwidth = tag->x_xres/2, xfull = tag->x_xres;
# endif
/*
 * Read back this quad.
 */
	XGetSubImage (tag->x_display, tag->x_sw[0], x, y, tag->x_xres/2, 
		tag->x_yres/2, ~0, ZPixmap, read, 0, 0);
/*
 * Zoom it.
 */
	for (row = 0; row < tag->x_yres/2; row++)
	{
		for (col = 0; col < xwidth; col++)
			*wps++ = Ztable[*rp++];
		memcpy (wps, wps - xwidth, xfull);
		wps += xwidth;
	}
/*
 * Write it back.
 */
 	XPutImage (tag->x_display, tag->x_sw[q], tag->x_tgt_gc, write, 0, 0,
		0, 0, tag->x_xres, tag->x_yres);
}





x11_calc_zoom (tag, q)
struct xtag *tag;
int q;
/*
 * Calculate the zoomed window for this quad, if necessary.
 */
{
	XImage *read, *write;
	int i;
/*
 * If a workspace has not been allocated, do so now.
 */
 	if (! tag->x_zwork)
		tag->x_zwork = getvm (tag->x_xres * tag->x_yres);
/*
 * Save the full screen before we start.
 */
	if (! tag->x_zoomok)
	{
		XCopyArea (tag->x_display, tag->x_window, tag->x_sw[0],
			tag->x_tgt_gc, 0, 0, tag->x_xres, tag->x_yres, 0, 0);
		for (i = 0; i < 5; i++)
			tag->x_qdone[i] = FALSE;
		tag->x_zoomok = TRUE;
	}
/*
 * If our quad is already done, we can quit now.
 */
 	if (tag->x_qdone[q])
		return;
/*
 * Create two XImage structures.  The "read" structure points to the last
 * quarter of our work space, while "write" uses the whole thing.  This
 * scheme allows quadrants to be zoomed "in place" with only one chunk
 * of memory.
 */
	read = XCreateImage (tag->x_display, tag->x_visual, 8, ZPixmap, 0,
		tag->x_zwork + ((3*tag->x_xres)/4)*tag->x_yres, tag->x_xres/2,
		tag->x_yres/2, 8, tag->x_xres/2);
	write = XCreateImage (tag->x_display, tag->x_visual, 8, ZPixmap, 0,
		tag->x_zwork, tag->x_xres, tag->x_yres, 8, tag->x_xres);
/*
 * Now do the quad of interest.
 */
 	x11_do_quad (tag, (q == 1 || q == 3) ? 0 : tag->x_xres/2,
		(q < 3) ? 0 : tag->x_yres/2, q, read, write);
	tag->x_qdone[q] = TRUE;
/*
 * All done.  Return our Ximages now.
 */
	read->data = write->data = 0;
	XDestroyImage (read);
	XDestroyImage (write);
}





x11_event (ctag)
char *ctag;
/*
 * Deal with any events which may have occurred.
 */
{
	struct xtag *tag = (struct xtag *) ctag;
	XEvent ev;
	int ret = 0;

	while (XCheckWindowEvent (tag->x_display, tag->x_window, 
		ExposureMask | KeyPressMask, &ev))
	{
		if (ev.type == KeyPress)
			x11_key (tag, &ev);
		else if (ev.type == Expose)
		{
			/* printf ("Expose reset\n"); */
			ret = 1;
		}
	}
	return (ret);
}




x11_key (tag, ev)
struct xtag *tag;
XEvent *ev;
/*
 * Deal with a key event.
 */
{
	static char string[20] = { 0 };
	KeySym key;

	XLookupString (ev, string, 20, &key, 0);
	switch (key)
	{
	   case XK_F1:	
		x11_vp ((char *) tag, 0, 1, 2, 2);
		break;
	   case XK_F2:	
		x11_vp ((char *) tag, 1, 1, 2, 2);
		break;
	   case XK_F3:	
		x11_vp ((char *) tag, 0, 0, 2, 2);
		break;
	   case XK_F4:	
		x11_vp ((char *) tag, 1, 0, 2, 2);
		break;
	   case XK_F5:
		x11_vp ((char *) tag, 0, 0, tag->x_xres, tag->x_yres);
		break;
	}
}



# endif /* DEV_X11 */
