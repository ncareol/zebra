/*HI***************************************************************************
 *
 * 	radinfo.h - radar information structures
 *
 ******************************************************************************
 *
 * HEADER INFORMATION
 *
 *	Software Suite		- RADEX
 *	Package			- Global
 *
 *	Reference number	- SP1/HDR/02000042
 *	
 *	Revision number		- $Revision: 2.1 $
 *	Release state		- $State: Exp $
 *
 *	Author, designer	- Ian McAfee
 *
 *	Modification date	- $Date: 1993-08-18 15:34:51 $
 *	Modified by		- $Author: burghart $
 * 
 * COPYRIGHT NOTICE
 *
 * 	Copyright (c) 1991 by Lassen Research
 *	All Rights Reserved
 *
 *	This program is copyright by Lassen Research, Chico, California,
 *	95928, (916) 343-6421.  It is licensed for use on a specific cpu
 *	and is not to be transferred or otherwise divulged.  Copies or
 *	modifications of this program must carry this copyright notice.
 * 
 * DESCRIPTION
 *
 *	Defines the radar information structures. Such as radar position,
 *	radar name etc
 *
 * MODIFICATION RECORD
 *
 * $Log: not supported by cvs2svn $
 * Revision 8.1  92/04/27  10:31:35  stafford
 * Version used during FAT
 * 
 * Revision 1.4  92/04/09  14:00:41  amca
 * Added module reference number
 * 
 * Revision 1.3  92/04/09  12:02:04  amca
 * After level three documentation header added and after a full merge of
 * the include directory with the latest code from Cowes. 
 * Full recompilation of RADEX made to ensure integrity of changes
 * 
 * Revision 1.2  92/04/03  14:25:33  kenb
 * After merging code from Chico
 * 
 * Revision 1.1  91/10/02  00:19:19  kenb
 * Initial revision
 * $Source: /code/cvs/rdss/zebra/source/src/ingest/radar/radinfo.h,v $
 *
 ******************************************************************************/


/**static char rcs_id[] = "$Id: radinfo.h,v 2.1 1993-08-18 15:34:51 burghart Exp $";*/

#ifndef RADINFO
#define RADINFO 1

struct radar_position	/* 6 bytes */
{
	short	degree;		/* north pos, south neg.		*/
	short	minute;		/* or east pos, west neg for longitude	*/
	short	second;
	short	filler;
};

struct radar_info	/* 32 bytes */
{
	char			radar_name[8];	/* 8 char name of radar	*/
	char			site_name[8];	/* 8 char name of site	*/
	unsigned short		antenna_height;	/* meters above sea level*/
	unsigned short		sparew;
	struct radar_position 	latitude;	/* latitude of ant.	*/
	struct radar_position 	longitude;	/* longitude of ant.	*/
};
#endif RADINFO

/*MODULE END*******************************************************************/
