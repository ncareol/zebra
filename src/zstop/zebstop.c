/*
 * Message server zapper.
 */
static char *rcsid = "$Id: zebstop.c,v 2.2 1992-03-17 22:37:23 barrett Exp $";

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
# include "../include/defs.h"
# include "message.h"
# include <copyright.h>

main ()
{
	int handler ();
	struct mh_template tm;

	if (! msg_connect (handler, "Grim reaper"))
		exit (0);
	tm.mh_type = MH_DIE;
	msg_send (MSG_MGR_NAME, MT_MESSAGE, 0, &tm, sizeof (tm));
	exit (0);
}



handler ()
{
	return (0);
}
