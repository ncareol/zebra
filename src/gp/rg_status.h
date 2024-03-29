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
 * RGRID return values
 */

# define RG_OK		0	/* Success 			*/
# define RG_NOTENUFPTS	1	/* Not enough good data points	*/
# define RG_COLLINEAR	2	/* Points are collinear		*/

/*
 * Useful place for this.  When g77 is in use, we need to add another
 * underscore to do_rgrid -- they changed the interface...sigh...
 */
# define do_rgrid_ do_rgrid__
