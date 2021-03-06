/* $Id: dm_cmds.h,v 2.15 2000-06-07 20:27:43 granger Exp $ */
/*
 * Keyword numbers for displaymgr commands.
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

# define DMC_CONFIG	1
# define DMC_DISPLAY	2
# define DMC_LIST	3
# define DMC_WINDOW	4
# define DMC_ENDCONFIG	5
# define DMC_ENDWINDOW	6
# define DMC_DESCRIPTION 7
# define DMC_NEWPD	8
# define DMC_BUTTONMAP	9
# define DMC_ENDMAP	10
# define DMC_EXCHANGE	11
# define DMC_BEEP	14
# define DMC_PDLOAD	15
# define DMC_PDDIR	16
# define DMC_PARAMETER	17
# define DMC_REMOVE	18
# define DMC_ADD 	19
# define DMC_HISTORY	20
# define DMC_REALTIME	21
# define DMC_COLORTABLE 22
# define DMC_ENDTABLE	23
# define DMC_XCOLOR	24
# define DMC_DM		25
# define DMC_LOCAL	26
# define DMC_IGNORE	27
# define DMC_MENU	28
# define DMC_DIAL	29
# define DMC_NONGRAPHIC 30
# define DMC_FORCEPD	31
# define DMC_TIME	32
# define DMC_PICKWIN	33
# define DMC_LINKPD	34
# define DMC_SHUTDOWN	35
# define DMC_WIDGET	36
# define DMC_SOUND	37
# define DMC_SHELL	38
# define DMC_RESTART	39
# define DMC_QUERY	90
# define DMC_REQUIRE    126
/*
 * Config editing and saving.
 */
# define DMC_CFGSAVE	40
# define DMC_CFGCONVERT	80
# define DMC_NEWCONFIG	41
# define DMC_NEWWINDOW	42
/*
 * Other stuff.
 */
# define DMC_ENTER	43
# define DMC_HELP	44
# define DMC_KILL	45
# define DMC_CYCLE	46
# define DMC_WINDOWLIST	47
/*
 * In-line plot descriptions
 */
# define DMC_BEGINPD	50
# define DMC_COMPPARAM	51
# define DMC_COMPONENT	52
# define DMC_GLOBAL	53
# define DMC_ENDPD	54
/*
 * Plot description utilities and feedback
 */
# define DMC_PDSHOW	60
# define DMC_PDWRITE	61
# define DMC_PDSTORE	62
# define DMC_PDCOPY	63
# define DMC_PDLIFT	64
# define DMC_PDDROP	65
/*
 * More newwindow optional keywords
 */
# define DMC_RENAME	70
# define DMC_REUSE	71
/*
 * Commands for dealing with process and window classes
 */
# define DMC_GRAPHIC	81	/* specify a graphic window in a config */
# define DMC_ENDGRAPHIC 82
# define DMC_PROCESS	83	/* specify a process window in a config */
# define DMC_PROTOTYPE	84	/* define a process class */
# define DMC_EXPLICIT	85	/* proto keyword for no implicit args */
# define DMC_FALLBACK	86	/* do not override existing definition */
# define DMC_REPLACE	87	/* redefine a process class, the default */
# define DMC_NEWGRAPHIC	88	/* add a graphic window to current config */

