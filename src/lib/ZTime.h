// Some -*- C++ -*- goodies for dealing with zebtimes.
//
// $Id: ZTime.h,v 2.1 1997-05-02 22:02:15 granger Exp $
//
/*		Copyright (C) 1987,88,89,90,91,92 by UCAR
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

#ifndef _ZTime_h_
#define _ZTime_h_

#include <defs.h>

inline
int operator< (const ZebTime &t1, const ZebTime &t2)
{
	return (t1.zt_Sec < t2.zt_Sec ||
		(t1.zt_Sec == t2.zt_Sec && t1.zt_MicroSec < t2.zt_MicroSec));
}


inline
int operator<= (const ZebTime &t1, const ZebTime &t2)
{
	return (t1.zt_Sec < t2.zt_Sec ||
		(t1.zt_Sec == t2.zt_Sec && t1.zt_MicroSec <= t2.zt_MicroSec));
}



inline
int operator> (const ZebTime &t1, const ZebTime &t2)
{
	return (t1.zt_Sec > t2.zt_Sec ||
		(t1.zt_Sec == t2.zt_Sec && t1.zt_MicroSec > t2.zt_MicroSec));
}



inline
int operator>= (const ZebTime &t1, const ZebTime &t2)
{
	return (t1.zt_Sec > t2.zt_Sec ||
		(t1.zt_Sec == t2.zt_Sec && t1.zt_MicroSec >= t2.zt_MicroSec));
}



inline
int operator== (const ZebTime &t1, const ZebTime &t2)
{
	return ((t1.zt_Sec == t2.zt_Sec) && 
		(t1.zt_MicroSec == t2.zt_MicroSec));
}


#endif /* ndef _ZTime_h_ */