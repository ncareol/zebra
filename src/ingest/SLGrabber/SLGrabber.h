/*
 * The serial link grabber protocol.
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

#ifndef _zebra_slgrabber_h_
#define _zebra_slgrabber_h_

# define SL_DATA	1			/* Data			*/

typedef struct _SLdata
{
	int		sl_type;		/* == SL_DATA		*/
	int		sl_len;			/* The length		*/
	unsigned char	sl_data[1];		/* Actual data		*/
} SLdata;

#endif /* _zebra_slgrabber_h_ */
