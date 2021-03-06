

#include "Logger.hh"
#include "BTreeFile.hh"
#include "SerialZTime.hh"

struct ds_DataFileCore
{
    char	dfc_name[50];		/* The name of the file	*/
    ZTime	dfc_begin;		/* When the data begins		*/
    ZTime	dfc_end;		/* When it ends			*/
    long	dfc_rev;		/* Revision count		*/
    ino_t       dfc_inode;		/* Inode number			*/
    int		dfc_ftype;		/* Type of this file		*/
    unsigned int dfc_nsample;		/* Sample count for this file	*/
};

class DataFileCore : public ds_DataFileCore
{
public:
    void translate( SerialStream& ss )
    {
	ss.cstring( dfc_name, sizeof( dfc_name ) );
	ss << dfc_begin << dfc_end << dfc_rev << dfc_inode
	   << dfc_ftype << dfc_nsample;
    }

    std::ostream& PutTo( std::ostream& s ) const
    {
	using std::endl;
	ZTime begin( dfc_begin ), end( dfc_end );
	s << dfc_name << endl;
	s << "from " << begin << " to " << end << endl;
	s << "revision: " << dfc_rev << ", inode: " << dfc_inode << 
	    ", type: " << dfc_ftype << ", samples: " << dfc_nsample << endl;
	return s;
    }

    bool operator == (const DataFileCore &dfc) const
    {
	return ((strcmp (dfc_name, dfc.dfc_name) == 0) &&
		(dfc_begin == dfc.dfc_begin) &&
		(dfc_end == dfc.dfc_end) &&
		(dfc_rev == dfc.dfc_rev) &&
		(dfc_ftype == dfc.dfc_ftype) &&
		(dfc_inode == dfc.dfc_inode) &&
		(dfc_nsample == dfc.dfc_nsample));
    }

    bool operator != (const DataFileCore &dfc) const
    {
	return ! operator== (dfc);
    }
};

SERIAL_STREAMABLE( DataFileCore );

inline std::ostream& 
operator <<(std::ostream& s, const DataFileCore& dfc)
{
    return dfc.PutTo( s );
}

