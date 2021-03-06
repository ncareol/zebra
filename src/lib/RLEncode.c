/*
 * Run length encoding/decoding stuff.
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

# include "defs.h"
# include "message.h"

MAKE_RCSID("$Id: RLEncode.c,v 2.4 1996-11-19 07:48:30 granger Exp $")

void
RL_Encode (data, dest, smax, dmax, sused, ndest)
unsigned char *data, *dest;
int smax, dmax, *sused, *ndest;
/*
 * Run-length encode a piece of data.
 * Entry:
 *	DATA	Is the source data.
 *	DEST	Is the destination array.
 *	SMAX	Is the length of the source array.
 *	DMAX	is the length of the destination array.
 * Exit:
 *	Data has been encoded until either (1) the source is exhausted,
 *	or (2) the destination is filled.  SUSED is the number of source
 *	bytes consumed; NDEST is the amount of data put into the destination
 *	array.
 */
{
	int count, inlit = FALSE, ninpack = 0, pcount = 0;
	unsigned char *runbegin = NULL, *cdest = dest;

	*ndest = dmax;
	while (ninpack < smax && pcount < dmax)
	{
	/*
	 * If we are doing literal stuff and this would be the last byte,
	 * we just finish it out.
	 */
	 	if (inlit && pcount == dmax - 1)
		{
			*cdest = *data;
			if ((*runbegin = cdest - runbegin) == 128)
				*runbegin = 0;
			*sused = ninpack + 1;
			return;
		}
	/*
	 * If there is only one byte left, and we're not in literal
	 * we have no room to start anything.
	 */
	 	else if (! inlit && pcount == dmax - 1)
		{
			(*ndest)--;
			*sused = ninpack;
			return;
		}
	/*
	 * Otherwise figure out if we can do a run from here.
	 */
	 	for (count = 0; count <= 128 && (count + ninpack) < smax &&
					data[count] == data[0]; count++)
			;
		count--;
	/*
	 * If there's enough of the same stuff here to do a run, put it in.
	 */
		if (count > 2)
		{
			if (inlit)
				*runbegin = cdest - runbegin - 1;
			inlit = FALSE;
			*cdest++ = 0x80 | (count == 128 ? 0 : count);
			*cdest++ = *data;
			data += count;
			pcount += 2; ninpack += count;
		}
	/*
	 * If no run, but in literal, make sure we've not filled it.
	 */
	 	else if (inlit && cdest - runbegin == 128)
		{
			*runbegin = 0;
			*cdest++ = *data++;
			pcount++; ninpack++; inlit = FALSE;
		}
	/*
	 * Otherwise we just add to the literal run, starting it if need be.
	 */
	 	else
		{
			if (! inlit)
			{
				runbegin = cdest++;
				inlit = TRUE;
				pcount++;
			}
			*cdest++ = *data++;
			pcount++; ninpack++;
		}
	}
/*
 * We filled things out.  End up a literal if necessary.
 */
	if (inlit)
		*runbegin = cdest - runbegin - 1;
	*ndest = pcount;
	*sused = ninpack;
	return;
}









void
RL_Decode (dest, src, len)
register unsigned char *dest;
unsigned char * const src;
const int len;
/*
 * Run-length decode this packet.
 */
{
	register unsigned char *sp = src;
	register int i;

	while (sp - src < len)
	{
	/*
	 * Look for the run bit.  If it's there, we replicate the next
	 * byte.
	 */
	 	if (*sp & 0x80)
		{
			const int runlen = (*sp & 0x7f) ? (*sp & 0x7f) : 128;
			const unsigned char c = *++sp;
			for (i = 0; i < runlen; i++)
				*dest++ = c;
			sp++;
		}
	/*
	 * Otherwise we have a literal run.
	 */
	 	else
		{
			const int runlen = *sp ? *sp : 128;
			sp++;
			for (i = 0; i < runlen; i++)
				*dest++ = *sp++;
		}
	}
}





