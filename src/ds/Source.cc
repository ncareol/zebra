//
// Source: a class holding a large list of files; each file can be retrieved
// by platform and time.  
//
//		Copyright (C) 1998 by UCAR
//	University Corporation for Atmospheric Research
//		   All rights reserved
//
// No part of this work covered by the copyrights herein may be reproduced
// or used in any form or by any means -- graphic, electronic, or mechanical,
// including photocopying, recording, taping, or information storage and
// retrieval systems -- without permission of the copyright owner.
// 
// This software and any accompanying written materials are provided "as is"
// without warranty of any kind.  UCAR expressly disclaims all warranties of
// any kind, either express or implied, including but not limited to the
// implied warranties of merchantibility and fitness for a particular purpose.
// UCAR does not indemnify any infringement of copyright, patent, or trademark
// through use or modification of this software.  UCAR does not provide 
// maintenance or updates for its software.
//
# include <errno.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>

# include <iostream>

# include <message.h>
# include <BTreeFile.cc>

# include "Source.hh"

#define CHECK 0

RCSID ("$Id")

//
// The order and size we want to use for our PlatFileList BTrees.  The
// given size is just a hint, allowing space for average file name 
// lengths of 28 bytes.  Longer names are of course still allowed.
// (This is klugy, since we really shouldn't know here that the DataFileCore
// uses CFG_DATAFILE_LEN bytes for the file name...)
//
// const int PFL_SIZE = sizeof( DataFileCore ) - CFG_DATAFILE_LEN + 28;
// const int PFL_ORDER = 32;
//
// For a while the size and order (32) originally specified were not used,
// and so the BTree defaults were used.  This just makes those settings
// explicit.  Someday we may want to tune them, or allow them to be tuned.

const int PFL_ORDER = 128;
const int PFL_SIZE = sizeof( DataFileCore );

//
// Construct a Source:
//	srcname		the name for this source
//	dir		base directory of the source
//	cachename	the name of the external representation block file
//
Source::Source( const char* in_srcname, const char* in_dir, 
		const char* cachename ) :
    bfile( cachename ),
    srcname( in_srcname ),
    rootdir( in_dir ),
    flags( 0 )
{
    int status = bfile.Status();
//
// If the initial file open came back with COULD_NOT_OPEN, try a
// read-only open.
//
    if (bfile.Status() == BlockFile::COULD_NOT_OPEN)
    {
	bfile.Open( cachename, 0, BlockFile::BF_READONLY );
	status = bfile.Status();
	if (status == BlockFile::OK)
	    msg_ELog( EF_INFO, 
		      "Opened source '%s' cache file with read-only access",
		      in_srcname );
    }
//
// If we've opened/created the block file successfully, reload/create the
// BTree of offsets to the per-platform datafile lists.  The root of the
// tree should be at the "application block" of the block file, so we don't
// give a block address.
//
// Other status values make us throw an error...
//
    switch (status)
    {
      case BlockFile::OK:
	poffsets = new OffsetTree( bfile );

	if (CHECK)
	{
	    // Try a check, then enable future checks.
	    poffsets->Check ();
	    poffsets->Check (1);
	    //	cerr << "Platform Offsets tree:" << endl;
	    //	poffsets->Print (cerr);
	}
	break;
      case BlockFile::COULD_NOT_OPEN:
	msg_ELog( EF_EMERGENCY, "Could not open cache %s for source '%s'",
		  cachename, in_srcname );
	throw Error();
      default:
	msg_ELog( EF_EMERGENCY, 
		  "Error BlockFile::%d opening cache %s for source '%s'",
		  status, cachename, in_srcname );
	throw Error();
    }
}



Source::~Source( void )
{
//
// Delete all the PlatFileLists we've instantiated.
//
    PlatFilesMap::const_iterator p;

    for (p = pflists.begin(); p != pflists.end(); p++)
    {
    // 
    // The "second" part of the key/value pair represented by p is the
    // PlatFileList* we want to delete
    //
	delete p->second;
    }
//
// Delete the poffsets BTree
//
    delete poffsets;
//
// Finally, close the block file
//
    bfile.Close();
}



    
bool
Source::AddPlatform( const Platform *p )
{
    if (! IsGood())
	return 0;

    string pname = pi_Name( p );

    if (poffsets->Find( pname ))
    {
	msg_ELog( EF_INFO, "Source: Attempt to re-add existing platform %s",
		  pname.c_str() );
	return 0;
    }
//
// Instantiate a PlatFileList for the platform, and save its offset
//
    pflists[pi_Id( p )] = new PlatFileList( 0, bfile, PFL_ORDER, PFL_SIZE );
    poffsets->Insert( pname, pflists[pi_Id( p )]->Address() );

    if (CHECK)
    {
	pflists[pi_Id( p )]->Check ();
	pflists[pi_Id( p )]->Check (1);
    }
    return 1;
}



bool
Source::RemovePlatform( const Platform *p )
{
    if (! IsGood())
	return 0;

    PlatformId pid = pi_Id( p );
    string pname = pi_Name( p );
//
// Complain if we don't know about the given platform
//
    if (! poffsets->Find( pname ))
    {
	msg_ELog( EF_INFO, "Source: Attempt to remove non-existent plat %s",
		  pname.c_str() );
	return 0;
    }
//
// Delete the platform's instantiated PlatFileList, if any, and remove the 
// platform's offset from our list of offsets
//
    if (pflists[pid])
    {
	delete pflists[pid];
	pflists[pid] = 0;
    }
    poffsets->Remove( pname );
    return 1;
}



bool
Source::AddFile( const Platform *p, const DataFileCore& dfc )
//
// Add a new file.  (This is just an update, with a complaint if the file
// is already here).
//
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );

    if (pfl->Find( dfc.dfc_begin ))
    {
	msg_ELog( EF_PROBLEM, 
		  "AddFile[%s]: %s overriding file with same start time",
		  pi_Name( p ), dfc.dfc_name );
    }
    
    return UpdateFile( p, dfc );
}



bool
Source::UpdateFile( const Platform *p, const DataFileCore& dfc )
//
// Add or replace a file.
//
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );

    // An insertion automatically replaces an existing key.
    pfl->Insert( dfc.dfc_begin, dfc );
    return 1;
}



bool
Source::RemoveFile( const Platform *p, const DataFileCore& dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    if (! pfl->Remove( dfc.dfc_begin ))
    {
	msg_ELog( EF_PROBLEM, 
		  "RemoveFile[%s]: attempt to remove nonexistent file %s",
		  pi_Name( p ), dfc.dfc_name );
	return 0;
    }
    return 1;
}
    
    
//
// FindBefore: If we have a file starting at or before 'zt', fill in 'dfc'
// with the latest such file and return non-zero.  Otherwise return zero
// and the contents returned in 'dfc' are undefined.
//
bool
Source::FindBefore( const Platform *p, const ZTime& zt, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;
//
// We key our list by begin time, and Find returns the first key at or
// after the desired key.  Hence, we take what we get if Find gets an exact
// hit (i.e., returns true), otherwise the file before the "near miss", if
// such a previous file exists.
//
    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Find( zt, dfc ) || pfl->Prev( 1, 0, dfc ) );
}

    
//
// FindAfter: If we have a file beginning at or after 'zt', fill in 'dfc' with
// the earliest such file and return non-zero.  Otherwise return zero and
// the contents returned in 'dfc' are undefined.
//
bool
Source::FindAfter( const Platform *p, const ZTime& zt, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;
//
// We key our list by begin time, and Find returns the first key at or
// after the desired key.  Hence, we just take what we get if Find
// gets an exact hit, otherwise the next file in the list (if any).
//
    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Find( zt, dfc ) || pfl->Next( 1, 0, dfc ) );
}



//
// FindExact: If we have a file starting exactly at 'zt', fill in 'dfc' with
// the file and return non-zero.  Otherwise return zero and the contents 
// returned in 'dfc' are undefined.
//
bool
Source::FindExact( const Platform *p, const ZTime& zt, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Find( zt, dfc ) );
}



bool
Source::First( const Platform *p, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->First( 0, dfc ) );
}



bool
Source::Last( const Platform *p, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Last( 0, dfc ) );
}



bool
Source::Prev( const Platform *p, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Prev( 0, dfc ) );
}



bool
Source::Next( const Platform *p, DataFileCore* dfc )
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return( pfl->Next( 0, dfc ) );
}



const Source::string&
Source::DataDir( const Platform *p )
//
// Return the full path of the directory we're using for the given platform.
// The returned string is valid until the next call to this method (from
// any instance).
//
{
    static string fullpath;

    if (! IsGood())
    {
	fullpath = "<no dir>";
	return (fullpath);
    }
//
// First get either the optional user-fixed platform directory from our list,
// or just use the platform's suggested directory.
//
    string pname( pi_Name( p ) );
    string pdir;

    if (optplatdirs[pname].length() != 0)
	pdir = optplatdirs[pname];
    else
	pdir = pi_SuggestedDir( p );
//
// If the directory we have is absolute, use it as-is, otherwise prepend
// our base directory
//
    if (pdir[0] == '/')
	fullpath = pdir;
    else
	fullpath = rootdir + "/" + pdir;

    return (fullpath);
}



int
Source::ConfirmDataDir( const Platform *p )
//
// Check whether this platform's data directory exists, and if not
// try to create it.
//
{
    if (! IsGood())
	return 0;

    const char* dir = DataDir( p ).c_str();
    
    if (access( dir, R_OK | W_OK | X_OK ) == 0)
	return 1;
    else if (errno == EACCES)
	msg_ELog( EF_PROBLEM, "Access denied to path %s", dir );
    else if (errno == ENOENT)
    {
    //
    // Make the directory if possible.
    // First try to make all of the parent directories, then try
    // for the whole thing.
    //
	char tmp[256];
	const char *slash = dir;

	while ((slash = (const char *) strchr (slash + 1, '/')))
	{
	    strncpy( tmp, dir, slash - dir );
	    tmp[slash - dir] = '\0';
	    mkdir (tmp, 0777);
	}

	if (mkdir (dir, 0777) == 0)
	{
	    msg_ELog( EF_INFO, "Created data dir %s (plat %s)", dir, 
		      pi_Name( p ) );
	    return 1;
	}
	else
	    msg_ELog( EF_PROBLEM, "Error %d creating dir %s (plat %s)",
		      errno, dir, pi_Name( p ) );
    }
    else
	msg_ELog( EF_PROBLEM, "access() error %d checking dir %s",
		  errno, dir );

    return 0;
}




void
Source::SetPlatDir( const string& platname, const string& dir )
//
// Set a directory overriding the default for the given platform
//
{
    if (IsGood())
	optplatdirs[platname] = dir;
}



	
int
Source::NFiles( const Platform *p )
//
// How many files do we know about for the given platform?
//
{
    if (! IsGood())
	return 0;

    PlatFileList *pfl = GetPlatFileList( p );
    return pfl->numKeys ();
}



void
Source::Dump ( std::ostream &out )
{
    using std::endl;
    out << "============ Platform offsets tree ===============" << endl;
    out << "Source: " << srcname << endl;
    out << "  Root: " << rootdir << endl;
    out << endl;
    poffsets->Print (out);

    for (PlatformId plat = 0; plat < dt_NPlatform(); plat++)
    {
	const Platform *p = dt_FindPlatform (plat);
	if (p)
	{
	    PlatFileList *pfl = GetPlatFileList (p);
	    out << "--------------------------------------------" << endl;
	    out << "Platform Name: " << pi_Name (p) << endl;
	    out << "  Platform Id: " << plat << endl;
	    out << endl;
	    pfl->Print (out);
	}
    }
}



// Private methods:

//
// GetPlatFileList: return a pointer to the specified platform's PlatFileList,
// creating a new one if necessary
//
PlatFileList*
Source::GetPlatFileList( const Platform *p )
{
    PlatformId pid = pi_Id( p );
//
// Use the parent's PlatFileList for subplatforms and others that share a 
// directory with their parent
//
    if (pc_InstanceDirFlag( pi_Class( p ) ) == InstanceCopyParent)
	return GetPlatFileList( pi_Parent( p ) );
//
// We have stuff to do if we haven't instantiated a BTree for this platform yet
//
    PlatFileList *pfl = pflists[pid];
    if (!pfl)
    {
	BlkOffset offset;
	string pname = pi_Name (p);
    //
    // If we have an offset for this platform in our list of offsets,
    // then recreate the existing BTree for the platform using that offset.
    //
	if (poffsets->Find( pname, &offset ))
	    pflists[pid] = new PlatFileList( offset, bfile );
    //
    // Otherwise, create a brand new BTree for the platform, and put its offset
    // into our offset list.
    //
	else
	    AddPlatform( p );

	pfl = pflists[pid];
	if (CHECK)
	{
	    pfl->Check ();
	    pfl->Check (1);
	    //	cerr << "Platform File List (" << pname << "): " << endl;
	    //	pfl->Print (cerr);
	}
    }
//
// Return the BTree from our list
//
    return pfl;
}
