/*
 * Dump SSM/I Antenna Temperature tapes from Remote Sensing Systems
 */
/*
 *		Copyright (C) 1993 UCAR
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

# ifndef lint
static char *rcsid = 
	"$Id: FindPositions.c,v 1.2 1993-06-07 18:34:06 granger Exp $";
# endif

# include <time.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>

# include "ssmi_record.h"


# define TRUE	-1
# define FALSE	0
# define EPOCH_1987	536457600	/* 00:00 Jan. 1, 1987 in UNIX epoch */


static int	Fd;		/* file descriptor for our tape drive */
static SSMI_Rec	Lrec[16];	/* array of logical records (one phys. rec.) */
static int	Nlog;

static int	nextrec ();



main ()
{
	char	tapedev[20];
	int	i;

	printf ("Tape device: ");
	gets (tapedev);

	if ((Fd = open (tapedev, O_RDONLY)) < 0)
	{
		printf ("Error %d opening device '%s'\n", errno, tapedev);
		exit (1);
	}
/*
 * Dump the header
 */
	printf ("\n\nTAPE HEADER\n-----------\n\n");

	nextrec ();
	for (i = 0; i < Nlog; i++)
		printf ("%79s\n", (char *)(Lrec + i));
/*
 * Data records
 */
	printf ("\n\nDATA RECORDS\n------------\n\n");
	while (nextrec ())
	{
		for (i = 0; i < Nlog; i++)
			dump_lrec (Lrec + i);
	}
}



static int
nextrec ()
/*
 * Read the next record from the tape into Lrec and put the number of
 * logical records into Nlog.  Return true for a good read, false for an
 * error or EOF.
 */
{
	int size = read (Fd, Lrec, sizeof (Lrec));
/*
 * Check for error or EOF
 */
	if (size <= 0)
	{
		if (size < 0)
			printf ("Error %d reading tape\n", errno);
		return (FALSE);
	}
	
	Nlog = size / sizeof (SSMI_Rec);
	return (TRUE);
}



static void
dump_lrec (rec)
SSMI_Rec *rec;
/*
 * Dump a logical record
 */
{
	int	i, t;
	char	*lo_types[] = {"19V", "19H", "37V", "37H", "22V"};
	char	*hi_types[] = {"85V", "85H"};

	printf ("Scan time: %s\n", ctime (EPOCH_1987 + rec->time_sec));
/*
 * Low frequency data
 */
	for (t = 0; t < 5; t++)
	{
		printf ("%s:\n", lo_types[t]);
		for (i = 0; i < 64; i++)
		{
			printf ("%5d", extract_12bit (rec->lo_data[i], t));
			if (((i+1) % 16) == 0)
				printf ("\n");
		}
	}
/*
 * High frequency A-scan data
 */
	for (t = 0; t < 2; t++)
	{
		printf ("A-scan %s:\n", lo_types[t]);
		for (i = 0; i < 64; i++)
		{
			printf ("%5d", extract_12bit (rec->hi_data[i], t));
			printf ("%5d", extract_12bit (rec->hi_data[i], t + 5));

			if (((i+1) % 16) == 0)
				printf ("\n");
		}
	}
/*
 * High frequency B-scan data
 */
	for (t = 0; t < 2; t++)
	{
		printf ("A-scan %s:\n", lo_types[t]);
		for (i = 0; i < 64; i++)
		{
			printf ("%5d", extract_12bit (rec->hi_data[i], t));
			printf ("%5d", extract_12bit (rec->hi_data[i], t + 5));

			if (((i+1) % 16) == 0)
				printf ("\n");
		}
	}
	printf ("\n");
}
