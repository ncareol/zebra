/*
 * Barb drawing routine.
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
# include <X11/Intrinsic.h>
# include <math.h>

# include "defs.h"
# include "draw.h"

RCSID ("$Id: barb.c,v 1.11 1997-10-23 15:35:53 burghart Exp $")

# define ARROWANG .2618 /* PI/12 */
#define DX(len,ang)	((double)len) * cos((double)(ang))
#define DY(len,ang)	((double)len) * sin((double)(ang))


void
draw_barb(W, D, Gcontext, x, y, angle, spd, shaftlen, doKnots )
Display *W;
Drawable D;
GC Gcontext;
int x, y;	    /* coordinate origin of barb in pixels */
double angle; 	    /* angle in radians (FROM WHICH the wind is blowing) */
double spd;  	    /* speed of wind */
int	shaftlen;   /* length in pixels to draw barb shaft */
int	doKnots;
/*
 *  Draw a wind-barb.
 */
{
	int xend, yend;
	float barbspd ;
	int x1,y1,x2,y2;
	XPoint	coord[5];
	int	flaglen = shaftlen / 3;
	float	shaftstep = shaftlen / 6;
	int	quadrant;
	int	dxsign = 0, dysign = 0;
	double flagangle = 0.0;
/*
 *  Draw the shaft of the barb.
 */
	quadrant = (int)( angle / 1.570796327 )+1;
	quadrant = quadrant % 4;
	switch( quadrant )
	{
	    case 0: /* quadrant 4 */
		flagangle = 2*3.141592654 - angle;
		dxsign = -1;
		dysign = -1;
	    break;
	    case 1: /* quadrant 1 */
		flagangle = angle;
		dxsign = -1;
		dysign = 1;
	    break;
	    case 2: /* quadrant 2 */
		flagangle = 3.141592654 - angle;
		dxsign = 1;
		dysign = 1;
	    break;
	    case 3: /* quadrant 3 */
		flagangle = angle - 3.141592654;
		dxsign = 1;
		dysign = -1;
	    break;
	}
	xend = (int)((double)x + DX(shaftlen, angle));
	yend = (int)((double)y - DY(shaftlen, angle));

	XDrawLine(W, D, Gcontext, x, y, xend, yend);
/* convert from m/s to knots if requested */
	if ( doKnots )
	    barbspd = spd / .5148 ;
	else
	    barbspd = spd;
/*
 * Leave some exposed shaft if the wind speed is < 7.5
 */
	if (barbspd < 7.5)
		shaftlen -= shaftstep;
/*
 *  Add the speed flags.
 */
	while ( barbspd >= 97.5 )
	{
	    coord[0].x = (int)((double)x + DX(shaftlen, angle));
	    coord[0].y = (int)((double)y - DY(shaftlen, angle));
	    coord[1].x = coord[0].x + 
		    dysign * (int)(DY(flaglen, flagangle));
	    coord[1].y = coord[0].y - 
		    dxsign * (int)(DX(flaglen, flagangle));
	    shaftlen -= shaftstep;
	    coord[3].x = (int)((double)x + DX(shaftlen, angle));
	    coord[3].y = (int)((double)y - DY(shaftlen, angle));
	    coord[2].x = coord[3].x + dysign*(int)(DY(flaglen, flagangle));
	    coord[2].y = coord[3].y - dxsign*(int)(DX(flaglen, flagangle));
	    XFillPolygon(W, D, Gcontext, coord, 4,Convex,CoordModeOrigin);
	    shaftlen -= shaftstep;
	    barbspd -= 100.0;
	}
	while ( barbspd >= 47.5 )
	{
	    coord[0].x = (int)((double)x + DX(shaftlen, angle));
	    coord[0].y = (int)((double)y - DY(shaftlen, angle));
	    coord[1].x = coord[0].x + 
		    dysign * (int)(DY(1.2 * flaglen, flagangle));
	    coord[1].y = coord[0].y - 
		    dxsign * (int)(DX(1.2 * flaglen, flagangle));
	    shaftlen -= 1.25 * shaftstep;
	    coord[2].x = (int)((double)x + DX(shaftlen, angle));
	    coord[2].y = (int)((double)y - DY(shaftlen, angle));
	    XFillPolygon(W, D, Gcontext, coord, 3,Convex,CoordModeOrigin);
	    shaftlen -= 0.75 * shaftstep;
	    barbspd -= 50.0;
	}
	while ( barbspd >= 7.5 )
	{
	    x1 = (int)((double)x + DX(shaftlen + shaftstep, angle));
	    y1 = (int)((double)y - DY(shaftlen + shaftstep, angle));
	    x2 = x1 + dysign*(int)(DY(flaglen, flagangle));
	    y2 = y1 - dxsign*(int)(DX(flaglen, flagangle));
	    x1 = (int)((double)x + DX(shaftlen, angle));
	    y1 = (int)((double)y - DY(shaftlen, angle));
	    XDrawLine(W, D, Gcontext, x1, y1, x2, y2);
	    shaftlen -= shaftstep;
	    barbspd -= 10.0;
	}
	if ( barbspd >= 2.5 )
	{
	    x1 = (int)((double)x + DX(shaftlen + shaftstep/2, angle));
	    y1 = (int)((double)y - DY(shaftlen + shaftstep/2, angle));
	    x2 = x1 + dysign*(int)(DY(flaglen/2, flagangle));
	    y2 = y1 - dxsign*(int)(DX(flaglen/2, flagangle));
	    x1 = (int)((double)x + DX(shaftlen, angle));
	    y1 = (int)((double)y - DY(shaftlen, angle));
	    XDrawLine(W, D, Gcontext, x1, y1, x2, y2);
	}
}

