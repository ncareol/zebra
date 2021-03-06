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
/*
 * User coordinate <--> pixel coordinate conversion macros
 *
 * The coordinates are defined as shown in the diagram below:
 *
 *		0		      Gwidth
 *	       _|_______________________|
 *	      0 |	       (fx1,fy1)|
 *		|	 __________/	|
 *		|	|	  /|	|
 *		|	| (ux1,uy1)|	|
 *		|	|	   |	|
 *		|	|	   |	|
 *		|	|	   |	|
 *		|	|(ux0,uy0) |	|
 *		|	|/_________|	|
 *		|	/		|
 *		|  (fx0,fy0)		|
 *		|			|
 *		|			|
 *		|			|
 *		|_______________________|
 *		|			| \_ ICONSPACE buffer for icons
 *      Gheight_|_______________________| /
 *
 *
 *
 *	The larger box is the X drawable being used and is defined
 *	by its width and height in pixels (pixwidth and pixheight).
 *	In X pixel coordinates, the origin is at the UPPER LEFT
 *	corner.
 *
 *	The smaller box is represented by two sets of coordinates:
 *	
 *	The "f" coordinates are fractional values representing the 
 *	subwindow relative to the entire drawable (less the lowest 
 *	ICONSPACE pixels, which are reserved for putting up icons).  
 *	The "f" coordinates range from (0.0,0.0) at the LOWER LEFT 
 *	corner to (1.0,1.0) at the upper right corner of the drawable.  
 *	Note that the origin is different from the origin of the X 
 *	pixel coordinates.  In the diagram above, (fx0,fy0) would be 
 *	about (0.3,0.4) and (fx1,fy1) would be about (0.8,0.9).
 *
 *	The "u" coordinates are the user coordinates to use for
 *	the subwindow.  The user coordinates of the lower left
 *	corner are (ux0,uy0) and the user coordinates of the upper
 *	right corner are (ux1,uy1).
 */

# include "LayoutControl.h"

#ifdef notdef
/*
 * Plotting space.  We leave an area for annotation on all sides
 */
# define F_X0 0.05
# define F_X1 0.85
# define F_Y0 0.05
# define F_Y1 0.90
#endif

# define F_X0 FX0
# define F_X1 FX1
# define F_Y0 FY0
# define F_Y1 FY1

#ifdef notdef
extern float FX0, FX1, FY0, FY1;
#endif

#ifdef notdef
# define LEGENDSPACE	((int) (0.15 * GWWidth (Graphics)))
# define ANNOTATESPACE	((int) (0.1 * GWHeight (Graphics)))
# define ICONSPACE	50
#endif

# define LEGENDSPACE	0.15
# define ANNOTATESPACE	0.1
# define ICONSPACE	50


#ifdef notdef
/*
 * Space reserved for icons, and resultant usable plot height, both in pixels
 */
extern int IconSpace;
# define USABLE_HEIGHT	(GWHeight (Graphics) - IconSpace)
#endif
# define USABLE_HEIGHT	(GWHeight (Graphics))

# define IXPIX(ux)	LC_XPIX((ux),Xlo,Xhi)
# define IYPIX(uy)	LC_YPIX((uy),Ylo,Yhi)

# define XPIX(ux)	((short) IXPIX(ux))
# define YPIX(uy)	((short) IYPIX(uy))

# define XUSER(xp)	LC_XUSER((xp),Xlo,Xhi)
# define YUSER(yp)	LC_YUSER((yp),Ylo,Yhi)

#ifdef notdef
/*
 * User coordinate to pixel coordinate macros.  The I- prefixed macros
 * return an int instead of casting to short.  Some algorithms need
 * pixel coordinates which don't fit into shorts.
 */
# define IXPIX(ux)	(nint ((float)(GWWidth (Graphics)) * \
	(((ux) - Xlo) / (Xhi - Xlo) * (F_X1 - F_X0) + F_X0)))

# define IYPIX(uy)	(nint ((float)(USABLE_HEIGHT) * \
	(1.0 - (((uy) - Ylo) / (Yhi - Ylo) * (F_Y1 - F_Y0) + F_Y0))))

# define XPIX(ux)	((short) IXPIX(ux))
# define YPIX(uy)	((short) IYPIX(uy))

/*
 * Pixel coordinate to user coordinate macros
 */
# define XUSER(xp)	((((xp)/(float)GWWidth(Graphics)) - F_X0) * \
				(Xhi - Xlo)/(F_X1 - F_X0) + Xlo) 

# define YUSER(yp)	(((USABLE_HEIGHT-(yp)-1) / (float)(USABLE_HEIGHT) - \
				F_Y0) * (Yhi - Ylo)/(F_Y1 - F_Y0) + Ylo)
#endif

