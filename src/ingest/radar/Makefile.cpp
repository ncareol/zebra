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
MFVERSION="$Id: Makefile.cpp,v 1.2 1991-09-26 18:03:07 gracio Exp $"

# ifdef sun
/*
 * Sun options
 */
CC=gcc
CFLAGS=  -g -O -I$(ZEBHOME)/fcc/include -I$(ZEBHOME)/rdss/include
LIBS=../../lib/libfcc.a -lnetcdf -lrdss -lXaw -lXt -lXmu -lXext -lX11 -ltermcap -lm
# endif


BINDIR=../../bin
LIBDIR=../../lib
HDIR=../../include

OBJS = radar_ingest.o mtape.o Input.o Rasterize.o StatusWidget.o

all:	radar_ingest radar_ingest.lf RadarStatus

install:	ds_consumer radar_ingest radar_ingest.lf RadarStatus
	install -c -m 04555 -o root radar_ingest $(BINDIR)
	install -c RadarStatus $(BINDIR)
	install -c -m 0444 radar_ingest.lf $(LIBDIR)
/*	install -c ds_consumer $(BINDIR) */

include:

radar_ingest:	$(OBJS)
	$(CC) $(CFLAGS) -o radar_ingest $(OBJS) $(LIBS)

ds_consumer:  ds_consumer.o
	$(CC) $(CFLAGS) -o ds_consumer ds_consumer.o $(LIBS)

RadarStatus:	RadarStatus.o
	$(CC) $(CFLAGS) -o RadarStatus RadarStatus.o $(LIBS)

radar_ingest.lf: radar_ingest.state
	uic < make-lf

clean:
	rm -f *~ radar_ingest *.o Makefile.bak ds_consumer

Makefile: mf

mf:
	mv Makefile Makefile~
	cp Makefile.cpp Makefile.c
	echo "# DO NOT EDIT -- EDIT Makefile.cpp INSTEAD" > Makefile
	cc -E Makefile.c >> Makefile
	rm -f Makefile.c
	make depend

depend:
	makedepend $(CFLAGS) *.c
