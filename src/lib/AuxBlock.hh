/*
 * The auxillary block base class from which BlockFile helper classes
 * can derive common functionality for serialization and syncing.
 *
 * $Id: AuxBlock.hh,v 1.7 1998-05-15 19:36:34 granger Exp $
 */

#ifndef _AuxBlock_hh_
#define _AuxBlock_hh_

#include "BlockObject.hh"
#include "BlockFileP.hh"
#include "Serialize.hh"

/* On block behavior:

   When read from the file, it is clean (un-marked) until it changes its
   state, at which time it mark()s itself.  The next write sync will write
   the block to the file, update the revision, and clear the mark.

   On read syncs, if the revision in memory differs from that in the file,
   the block is read and the mark is cleared.

   When created but not yet allocated or written to the file, it starts
   out marked, and possibly without a block address or length.  A write sync
   may first allocate space in the file for the block before writing it.

   */

/*
 * The Auxiliary Block class uses the BlockObject classes to implement 
 * sync functionality for BlockFile auxiliary structures.  The FreeList
 * and Journal classes are derived from AuxBlock.
 */

class AuxBlock : virtual public RefBlock, virtual public SerialBlock
{
public:
	AuxBlock ()
	{
		// cout << "AuxBlock constructor" << endl;
	}

	virtual ~AuxBlock ()
	{ }

private:
	// Not implemented //
	AuxBlock (const AuxBlock &);
	AuxBlock &operator= (const AuxBlock &);
};



/*
 * Free blocks are just length and offset, without a revision.
 */
class FreeBlock
{
public:
	FreeBlock (BlkOffset addr = 0, BlkSize size = 0) :
		offset(addr), length(size)
	{ }

	BlkOffset offset;	/* Location of block */
	BlkSize length;		/* Length of block */

	void translate (SerialStream &ss)
	{
		ss << offset << length;
	}
};

SERIAL_STREAMABLE(FreeBlock);


struct FreeStats
{
	long nfree;		// Number of free blocks in the list
	long bytesfree;		// Number of bytes in free blocks
	long nrequest;		// Number of block requests
	long bytesrequest; 	// Number of bytes requested
	long bytesalloc;	// Number of bytes allocated
	long bytesfreed;	// Number of bytes freed altogether

	FreeStats () :
		nfree(0),
		bytesfree(0),
		nrequest(0),
		bytesrequest(0),
		bytesalloc(0),
		bytesfreed(0)
	{ }

	void translate (SerialStream &ss)
	{
		ss << nfree << bytesfree << nrequest;
		ss << bytesrequest << bytesalloc << bytesfreed;
	}
};

SERIAL_STREAMABLE(FreeStats);


//
// Package free list memory management into a convenient structure
//
class FreeList : virtual public AuxBlock
{
public:
	FreeList (BlockFile &bf, Block &b, SyncBlock *parent);
	~FreeList ();

	/*
	 * Add a freed block to the list.
	 */
	void Free (BlkOffset offset, BlkSize length);

	/*
	 * Find a block for this request and take it off the list.
	 * Return non-zero on success, zero otherwise.
	 */
	BlkOffset Request (BlkSize length, BlkSize *ret_length);

	FreeStats Stats ()
	{
		return (stats);
	}

	// Serialization interface
	int encode (SerialBuffer &sbuf);
	int decode (SerialBuffer &sbuf);
	long encodedSize (SerialBuffer &sbuf);

	// Inherit the growth allocation method

private:
	int ncache;	/* Number of free blocks allocated space in array */
	int n;		/* Actual number of free blocks in use in array */
	FreeBlock *blocks; /* The actual array of free blocks */

	// Statistics
	FreeStats stats;

	void Remove (int);
	void Add (BlkOffset offset, BlkSize length);
	void growCache (int num);

};



/* =================
 * Journal structure
 *
 * Describes and locates a change to the file and the new revision number
 * it resulted in.  Applications can use the journal to determine if an
 * in-memory cache of a block has changed and needs to be re-read.
 */


class JournalEntry;

class Journal : virtual public AuxBlock
{
public:
	typedef int ChangeType;

#ifdef notdef	// Sun CC can't handle this!
	static const long MaxEntries = 256;

	static const ChangeType BeginTransaction = 0;
	static const ChangeType BlockRemoved = 1;
	static const ChangeType BlockAdded = 2;
	static const ChangeType BlockChanged = 3;
	static const ChangeType EndTransaction = 4;
#endif
	static const long MaxEntries;

	static const ChangeType BeginTransaction;
	static const ChangeType BlockRemoved;
	static const ChangeType BlockAdded;
	static const ChangeType BlockChanged;
	static const ChangeType EndTransaction;

	Journal (BlockFile &bf, Block &b, SyncBlock *parent);
	~Journal ();

	// Functionality
	int Changed (BlkVersion, BlkOffset, BlkSize);
	void Record (ChangeType, BlkOffset, BlkSize);

	// Serialization interface
	int encode (SerialBuffer &sbuf);
	int decode (SerialBuffer &sbuf);
	long encodedSize (SerialBuffer &sbuf);

	// Don't grow; our queue size is fixed
	BlkSize grow (BlkSize need)
	{
		return need;
	}

private:
	int serialSize;		// Fixed, so calculated once
	int max;		// Size of queue
	int first;		// Beginning of ciruclar queue
	int last;		// End of circular queue
	JournalEntry *entries;
};


struct JournalEntry // : public Translatable
{
	Journal::ChangeType change;
	Block block;			/* Region changed */

	void translate (SerialStream &ss)
	{
		ss << change << block;
	}

	JournalEntry () : change(0), block()
	{ }
};

SERIAL_STREAMABLE(JournalEntry);



#endif /* _AuxBlock_hh_ */


