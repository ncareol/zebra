/*
 * Data store daemon-specific definitions.
 */
/* $Id: dsDaemon.h,v 3.11 1994-04-15 22:28:12 burghart Exp $ */
/*
 * The platform and data tables, via pointer.
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

#include <defs.h>

#ifndef _zeb_dsDaemon_h_
#define _zeb_dsDaemon_h_

Platform *PTable;
DataFile *DFTable;
int NDTEUsed;		/* How many data table entries used. */
int DTFreeList;		/* The datafile free list		*/
int NPlatform;		/* How many platforms			*/

/*
 * The default data directory.
 */
# define DDIR_LEN	CFG_FILEPATH_LEN

char DefDataDir[DDIR_LEN];
char RemDataDir[DDIR_LEN];


/*
 * This is a kludge to make it easier to keep a uniform init file.  If this
 * flag is set, no remote directories will be accessed.
 */
extern bool DisableRemote;

/*
 * Allow the revision numbering to be chosen from the config file.
 */
extern bool StatRevisions;

/*
 * Cache options.
 */
extern bool LDirConst;		/* Nothing changes		*/
extern bool RDirConst;
extern bool LFileConst;		/* Files don't change (but they	*/
extern bool RFileConst;		/* can come and go)		*/
extern bool CacheOnExit;	/* Write cache on way out?	*/

/*
 * This variable is TRUE only during the initial file scan.
 */
extern bool InitialScan;	/* True implies first data scan	*/
extern long LastScan;		/* Time of latest full scan	*/

/*
 * Metadata space
 */
extern int PTableSize, PTableGrow, DFTableSize, DFTableGrow;

/*
 * Internal functions.
 */
void InitSharedMemory FP ((void));
/*
 * Data table routines.
 */
Platform *dt_NewPlatform FP ((char *));
Platform *dt_FindPlatform FP ((char *, int));
void dt_SearchPlatforms FP ((int (*func)(), void *arg, bool sort, char *re));
DataFile *dt_NewFile FP ((void));
void dt_FreeDFE FP ((DataFile *));
void dt_AddToPlatform FP ((Platform *, DataFile *, int));
void dt_RemoveDFE FP ((Platform *, int));

void dc_DefPlatform FP ((char *));
void dap_Init FP ((void));
void dap_Request FP ((char *, struct dsp_NotifyRequest *));
void dap_Cancel FP ((char *, struct dsp_Template *));
void dap_Notify FP ((PlatformId, ZebTime *, int, int, int));
void dap_Copy FP ((char *));
void ClearLocks FP ((Platform *));
void CacheInvalidate FP ((int));
void Shutdown FP ((void));
void DataFileGone FP ((DataFile *df));

/*
 * Datascan.
 */
void	DataScan FP ((void));
void	Rescan FP ((PlatformId platid, int all));
void	WriteCache FP ((struct ui_command *));
void	ReadCacheFile FP ((char *, int));
<<<<<<< dsDaemon.h
void	RescanPlat FP ((Platform *));
||||||| 3.9.2.1
=======

#endif /* ! _zeb_dsDaemon_h_ */
>>>>>>> /tmp/T4a06562
