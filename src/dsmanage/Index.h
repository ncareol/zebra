//
// The file index class.
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

//
// The description of a single file.
//
class IndexFile
{
	IndexFile	*if_next;		// Next file in chain
	IndexFile	*if_same;		// Other entries for same file
	char *		if_name;		// Name of the file
	char *		if_plat;		// Platform name
	int		if_size;		// How big it is.
	ZebTime		if_begin;		// When it begins.
	ZebTime		if_end;			// When it ends.
	int		if_fileno;		// (tape) file number
	int		if_marked;
public:
	IndexFile (const char *plat, const char *name, int size, int filenum,
			const ZebTime *begin, const ZebTime *end);
	IndexFile (const IndexFile &);		// Initialization
	~IndexFile () { delete[] if_plat; delete[] if_name; }
//
// Basic info.
//
	const char * name () const { return if_name; }	// Get name of file
	const char * plat () const { return if_plat; }	// Get name of plat
	const ZebTime &begin () const { return if_begin; } // begin time
	const ZebTime &end () const { return if_end; };	// End time
	const int size () const { return if_size; } // how big?
	int filenum () const { return if_fileno; }	// Tape file number
	IndexFile *next () const { return if_next; }
	IndexFile *same () const { return if_same; }
	void link (IndexFile &next) { if_next = &next; }; // Link into chain
	void dup (IndexFile &chain) { if_same = &chain; };
	int& isMarked () { return if_marked; }
};


//
// Platform info structure.  Used to be inside the PlatformIndex class,
// but gcc-2.5 was not pleased with that...
//
struct PlatInfo
{
	ZebTime pi_begin, pi_end;
	IndexFile *pi_files;
	int pi_marked;
};

//
// Platform index type.
//
class PlatformIndex
{
	friend STTraverseProc ZapPlat;
	STable table;			// table containing platforms.
	STable dirs;			// table containing directories.
	struct PlatInfo *findPlat (const char *name) const;
	static int TNum;		// To make unique stbl names
	void FindDuplicates (IndexFile &file);
	int SameFile (const char *name, IndexFile *file);
public:
	PlatformIndex ();
	PlatformIndex (const char *);		// Load from file.
	~PlatformIndex ();
	void add (const char *, IndexFile &);
	void add (const char *);
	int save (const char *) const;		// Save to file
	int coverage (const char *, ZebTime &, ZebTime &) const;
	int& isMarked (const char *);
	IndexFile *files (const char *) const;
};
