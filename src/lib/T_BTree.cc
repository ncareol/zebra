/* -*- mode: c++; c-basic-offset: 8; -*-
 * Test the BTree
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#include <time.h>	// Need time() to seed srand()
#include <string>

using namespace std;

#include "T_BTree.hh"

#ifdef RCSID
RCSID("$Id: T_BTree.cc,v 1.36 2002-12-20 15:59:18 granger Exp $")
#endif

typedef BTreeFile<ZTime,DataFileCore> TimeFileTree;
typedef BTreeFile<string,string> StringFileTree;
typedef BTreeFile<long,long> LongFileTree;

typedef BTree<ZTime,ZTime> TimeTree;
typedef BTree<string,string> StringTree;
typedef BTree<long,long> LongTree;

/*
 * Choose the default tree type.
 */
typedef long test_key;
//typedef string test_key;
//typedef ZTime test_key;

typedef TimeFileTree default_tree;

static const int Debug = 0;


// ----------------------------------------------------------------------
// To test a tree, use sequential and random generators of integers.  Try
// inserting in order, then in reverse, then randomly.  After each
// insertion sequence, verify that all the keys can be found and have the
// correct values.  Make sure traversing the tree forward and reverse
// returns the correct keys and values.  Reinsert all the keys with a new
// value and verify those values.  Then remove all the keys randomly, and
// verify the tree is empty after it all.
// ----------------------------------------------------------------------


/*
 * Use a template of tree type so we can get the correct stats type.
 */
template <class T>
void
Summarize (ostream &out, T &t)
{
	out << " ----- " << endl;
	// typename T::Stats cs;
	// t.currentStats (cs);
	t.reportStats (out);
	out << " ----- " << endl;

#ifdef notdef
	BTreeStats s;
	t.collectStats (s);

	out << "Number of elements: " << s.numElements() << endl;
	out << "   Number of nodes: " << s.numNodes() << endl;
	out << "    Number of keys: " << s.numKeys() << endl;
	out << " Avg keys per node: " << s.averageKeys() << endl;
	out << " Max keys per node: " << s.maxKeys() << endl;
	out << " Min keys per node: " << s.minKeys() << endl;
	out << "   Total key slots: " << s.totalSlots() << endl;
	out << "  Key slots unused: " << s.keysUnused() << endl;
	out << " Elem slots unused: " 
	    << (t.Order()*s.numLeaves() - s.numElements()) << endl;
	out << "  Memory allocated: " << s.memoryAlloc() << " bytes." << endl;
	out << "       Memory used: " << s.memoryUsed() << " bytes." << endl;
	out << "   Pct memory used: " << s.percentMemory() << "%" << endl;
	out << "  Node memory used: " 
	    << s.percentNodeMemory() << "%" << endl;
	out << "Element buffer use: " 
	    << s.percentElementMemory() << "%" << endl;
	out << "Pct key slots used: " << s.percentSlots() << "%" << endl;
#endif
}

//#define Summarize(a,b)


/*
 * Simple generator for keys.
 */
template <class K>
struct Counter
{
	typedef K result_type;
	typedef unsigned long counter_type;

	Counter (counter_type _n = counter_type()) : n(_n) {}
	K operator()();

	counter_type n;
};


template <class K>
K Counter<K>::operator()()
{
	return K(n++);
}


template <>
string
Counter<string>::operator()()
{
	string s;
	++n;
	if (! (cin >> s))
	{
		char buf[16];
		sprintf (buf, "zzz%08ld", n);
		return buf;
	}
	return s;
}


template <>
ZTime
Counter<ZTime>::operator()()
{
    n += 3600;
    return n;
}


// Generate DataFileCore values
template <>
DataFileCore 
Counter<DataFileCore>::operator()()
{
	string s;
	++n;
	unsigned long t = n*3600;
	if (! (cin >> s))
	{
		char buf[16];
		sprintf (buf, "zzz%08ld", t);
		s = buf;
	}
	DataFileCore dfc;
	strcpy (dfc.dfc_name, s.c_str());
	dfc.dfc_begin = t;
	dfc.dfc_end = t + 3599;
	dfc.dfc_nsample = 3599;
	dfc.dfc_rev = n*n;
	dfc.dfc_inode = (ino_t)(1) << (n % 64);
	dfc.dfc_ftype = 99;
	return dfc;
}


template <class T>
struct tree_member // predicate
{
	typedef typename T::key_type argument_type; 
	typedef bool result_type;

	bool operator() (const argument_type &key) const
	{
		return (tree.Find (key));
	}
	tree_member (T &tree_) : tree(tree_)
	{ }

private:
	T &tree;
};



/*
 * Template for returning a default value for a type.
 */
template <typename T>
T
getDefault()
{
	return T();
}


template <>
ZTime
getDefault<ZTime>()
{
	ZTime zt(0);
	return zt;
}


/*
 * Put the test routines in a template class so we can call multiple
 * tree types within the same test.
 */
template <class test_tree>
class TreeTest
{
public:

	typedef typename test_tree::key_type key_type;
	typedef typename test_tree::value_type value_type;

int 
T_Members (test_tree &tree, vector<key_type> &keys)
{
	// Verify that all of the keys can be found in the tree
	int err = 0;
	typename vector<key_type>::iterator k;

	tree_member<test_tree> member(tree);
	for (k = keys.begin(); k != keys.end(); ++k)
	{
		if (! member(*k))
			break;
	}
	if (k != keys.end())
	{
		cout << "***** Member error: missing key: " << *k << endl;
		++err;
	}
	return err;
}



int
T_Reopen (test_tree &tree)
{
	cout << " re-opening tree..." << endl;
	tree.Reopen();
	if (tree.Error())
	{
		cout << "***** Error on Reopen." << endl;
		return (1);
	}
	return (0);
}



int
T_Compare (test_tree &tree, vector<key_type> &keys, 
	   vector<value_type> &values)
{
	// Given a tree, keys, and values, verify the tree contains all
	// the keys and has the correct values
	int err = 0;
	typename vector<key_type>::iterator k;
	typename vector<value_type>::iterator v;
	value_type value;

	for (k = keys.begin(), v = values.begin(); k != keys.end();
	     ++k, ++v)
	{
		if (! tree.Find (*k, &value))
		{
			++err;
			cout << "***** Compare error: missing key: "
			     << *k << endl;
		}
		else if (value != *v)
		{
			++err;
			cout << "***** Compare error: key: "
			     << *k << " has value " << value
			     << " instead of " << *v << endl;
		}
	}
	return err;
}



int 
T_Insert (test_tree &tree, vector<key_type> &keys, vector<value_type> &values)
{
	int err = 0;
	typename vector<key_type>::iterator k;
	typename vector<value_type>::iterator v;

	// Do the insertions
	//cout << " ...inserting keys: ";
	for (k = keys.begin(), v = values.begin(); k != keys.end();
	     ++k, ++v)
	{
		//cout << "(" << *k << ", " << *v << ")";
		tree.Insert (*k, *v);
		if (tree.Error())
		{
			cout << "***** Insert: internal check failure, "
			     << "key " << *k << ", value " << *v << endl;
		}
		if (Debug) tree.Print (cout);
	}
	//cout << endl;

	//cout << "----- Snapshot -----" << endl;
	//tree.Print (cout);

	Summarize (cout, tree);

	if (T_Reopen (tree))
		return (err+1);

	// Verify membership
	cout << " ...testing membership" << endl;
	err += T_Members (tree, keys);

	// Compare values
	cout << " ...comparing values" << endl;
	err += T_Compare (tree, keys, values);

	return (err);
}



int
T_Removal (test_tree &tree, 
	   typename vector<key_type>::iterator k, 
	   typename vector<key_type>::iterator last, int check_empty = 1)
{
	// As removing, make sure the removed key cannot be found
	//cout << " ...removing key: ";
	int err = 0;
	for ( ; k != last; ++k )
	{
		//cout << *k << " ";
		if (! tree.Remove (*k))
		{
			cout << "***** Removal: key missing: " << *k << endl;
			if (Debug) tree.Print (cout);
			++err;
		}
		if (tree.Error())
		{
			cout << "***** Removal: internal check failure, "
			     << "key " << *k << endl;
			++err;
		}
		value_type v = getDefault<value_type>();
		if (tree.Find (*k, &v))
		{
			cout << "***** Removal: removed key still found: "
			     << *k << " value: " << v << endl;
			++err;
		}
#ifdef notdef
		else if (v != getDefault<value_type>())
		{
			// Find() should return false without changing
			// the value parameter.
			cout << "***** Removal: key not found: "
			     << *k << " but value still set: " << v << endl;
			++err;
			v = getDefault<value_type>();
		}
#endif
		if (Debug) tree.Print (cout);
	}
	//cout << endl;
	tree.Check ();
	if (check_empty && ! tree.Empty ())
	{
		cout << "***** Removal: tree not empty. " << endl;
		if (Debug) tree.Print (cout);
		++err;
	}
	return (err);
}



int
T_RandomRemoval (test_tree &tree)
{
	// Build a vector of keys by traversing the tree, then shuffle
	// and remove.  We should be empty when its over.
	vector<key_type> keys;
	key_type key;

	tree.First ();
	//cout << " ...removal finding keys: ";
	while (tree.Current (&key))
	{
		keys.push_back (key);
		//cout << key << " ";
		tree.Next ();
	}
	//cout << endl;

	random_shuffle (keys.begin(), keys.end());
	return (T_Removal (tree, keys.begin(), keys.end()));
}




int
T_PartialRemoval (test_tree &tree, int n)
{
	int err = 0;
	// Build a vector of keys by traversing the tree, then shuffle
	// and remove n keys.  Report statistics, then re-insert those
	// n keys.
	//typedef pair<test_tree::key_type,test_tree::value_type> Pair; 
	//vector<Pair> entries;
	vector<key_type> keys;
	vector<value_type> values;
	key_type key;
	value_type val;
	typename vector<key_type>::iterator k;

	tree.First ();
	//cout << " ...removal finding keys: ";
	while (tree.Current (&key))
	{
		keys.push_back (key);
		//cout << key << " ";
		tree.Next ();
	}
	//cout << endl;

	random_shuffle (keys.begin(), keys.end());
	random_shuffle (keys.begin(), keys.end());
	for (k = keys.begin(); k != keys.end(); ++k)
	{
		err += (! tree.Find (*k, &val));
		values.push_back (val);
	}

	err += T_Removal (tree, keys.begin(), keys.begin()+n, 0);
	Summarize(cout, tree);
	if (Debug) tree.Print (cout);

	// Now re-insert everything; some will be overwrites
	err += T_Insert (tree, keys, values);
	Summarize(cout, tree);

	return (err);
}




int
T_ReverseRemoval (test_tree &tree)
{
	// Build a vector of keys by traversing the tree backwards.
	vector<key_type> keys;
	key_type key;

	tree.Last ();
	//cout << " ...removal finding keys: ";
	while (tree.Current (&key))
	{
		keys.push_back (key);
		//cout << key << " ";
		tree.Prev ();
	}
	//cout << endl;
	return (T_Removal (tree, keys.begin(), keys.end()));
}



// Search for keys not in the tree and see if traversal still works.
int
T_OutsideSearch (test_tree &tree)
{
    int err = 0;
    key_type key;
    value_type value, checkval;

    cout << "Testing keys outside tree range." << endl;
    if (! tree.First (&key, &value))
	return ++err;
    if (! tree.Remove())
	return ++err;
    key_type lowest;
    if (! tree.First (&lowest))
	return ++err;

    // Now we have a key lower than all other keys in the tree.
    if (tree.Find (key))
    {
	cout << "***** Low key should not have been found." << endl;
	return ++err;
    }
    // After a find on a lower key we still expect the cursor to be valid
    // (current), but no value should be available because the key does not
    // exist.
    else if (! tree.Current () || tree.Current (0, &checkval))
    {
	++err;
	cout << "***** Should have valid cursor but no value" 
	     << " after failed find." << endl;
    }
    // Now try to advance to a valid key.
    tree.Next();
    key_type found;
    if (! tree.Current(&found))
    {
	++err;
	cout << "***** Next key after search key not found." << endl;
    }
    else if (found != lowest)
    {
	++err;
	cout << "***** Expected next key to be lowest key in tree." << endl;
    }
    // Return the key when done.
    tree.Insert (key, value);

    // The same as above but for the end of the tree.
    if (! tree.Last (&key, &value))
	return ++err;
    if (! tree.Remove())
	return ++err;
    key_type highest;
    if (! tree.Last (&highest))
	return ++err;

    if (tree.Find (key))
    {
	cout << "***** High key should not have been found." << endl;
	++err;
    }
    else if (! tree.Current () || tree.Current (0, &checkval))
    {
	++err;
	cout << "***** Should have valid cursor but no value" 
	     << " after failed find." << endl;
    }
    tree.Prev();
    if (! tree.Current(&found))
    {
	++err;
	cout << "***** Previous key to search key not found." << endl;
    }
    else if (found != highest)
    {
	++err;
	cout << "***** Expected prev key to be highest key in tree." << endl;
    }
    tree.Insert (key, value);
    return err;
}



int
T_Traversal (test_tree &tree, int print = 0)
{
	// Build an ordered vector of keys by traversing the tree forwards.
	vector<key_type> keys;
	typename vector<key_type>::iterator iv;
	key_type key;
	int err = 0;

	tree.First ();
	if (print)
		cout << "Traversal keys: ";
	while (tree.Current (&key))
	{
		if (print)
			cout << key << " ";
		keys.push_back (key);
		tree.Next ();
	}
	if (print) cout << endl;

	// Now do the same traversal backwards, and make sure we get the
	// same keys coming the other direction.
	tree.Last ();
	iv = keys.end();
	while (iv != keys.begin() && tree.Current (&key))
	{
		--iv;
		if (*iv != key)
		{
			cout << "***** Traversal backwards: expected key "
			     << *iv << ", got key " << key << endl;
			++err;
		}
		tree.Prev ();
	}
	if (iv != keys.begin() || tree.Current())
	{
		++err;
		cout << "***** Traversal backwards: did not exhaust tree"
		     << endl;
	}
	
	// Now traverse forwards in increments of 1/10 the size.
	iv = keys.begin();
	int step = keys.size() / 10 + 1;
	cout << " ...stepping forward by " << step << ": ";
	tree.First ();
	while (tree.Current (&key))
	{
		cout << key << " ";
		if (*iv != key)
		{
			cout << "***** Traversal error: expected key "
			     << *iv << ", got key " << key << endl;
			++err;
		}
		iv += step;
		tree.Next (step);
	}
	cout << endl;
	
	// Now traverse backwards in increments of 1/13 the size.
	iv = keys.end() - 1;
	step = keys.size() / 13 + 1;
	cout << " ...stepping backwards by " << step << ": ";
	tree.Last ();
	while (tree.Current (&key))
	{
		cout << key << " ";
		if (*iv != key)
		{
			cout << "***** Traversal error: expected key "
			     << *iv << ", got key " << key << endl;
			++err;
		}
		iv -= step;
		tree.Prev (step);
	}
	cout << endl;
	return err;
}



int
T_RandomAccess (test_tree &tree, 
		vector<key_type> &keys,
		vector<value_type> &values,
		int print = 0)
{
	int err = 0;
	int ntimes = 5*tree.numKeys();
	int nkeys = keys.size();
	typename vector<key_type>::iterator k;
	typename vector<value_type>::iterator v;
	value_type val;

	cout << "Random accesses (" << ntimes << ")..." << endl;
	srandom (42);
	for (int i = 0; i < ntimes; ++i)
	{
	    int r = (random() % nkeys);
	    err += (! tree.Find (keys[r], &val));
	    err += (val != values[r]);
	}
	return err;
}




int
TestTree (test_tree &tree, int N)
{
	// Put a tree through its paces.  Check() it after major operations.

	int err = 0;
	vector<key_type> keys(N);
	vector<value_type> values(N);
	generate (keys.begin(), keys.end(), Counter<key_type>(1));
	generate (values.begin(), values.end(), Counter<value_type>(1));

	// Insert the sequential keys and values and test.
	cout << "Sequential insert... " 
	     << *keys.begin() << " to " << keys.back() << endl;
	err += T_Insert (tree, keys, values);
	err += tree.Check ();
	Summarize(cout, tree);
	err += T_Traversal (tree);

	if (T_Reopen (tree))
		return (err+1);
	err += T_Traversal (tree);

	// Random accesses (ie datastore file access)
	err += T_RandomAccess (tree, keys, values);

	if (T_Reopen (tree))
		return (err+1);

	// Random accesses (ie datastore file access)
	err += T_RandomAccess (tree, keys, values);

	// Searches out of bounds.
	err += T_OutsideSearch (tree);

	// Ordered removal
	cout << "Forward removal... " << endl;
	err += T_Removal (tree, keys.begin(), keys.end());
	err += tree.Check ();

	// Insert the sequential keys and values and test.
	cout << "Sequential insert... " 
	     << *keys.begin() << " to " << keys.back() << endl;
	err += T_Insert (tree, keys, values);
	err += tree.Check ();

	// Reverse removal
	cout << "Reverse removal... " << endl;
	err += T_ReverseRemoval (tree);
	err += tree.Check ();

	// Insert the sequential keys and values and test.
	cout << "Sequential test and erase... " 
	     << *keys.begin() << " to " << keys.back() << endl;
	err += T_Insert (tree, keys, values);
	err += tree.Check ();
	tree.Erase ();

	cout << "Reopening after Erase()..." << endl;
	if (T_Reopen (tree))
		return (err+1);

	if (! tree.Empty())
	{
		++err;
		cout << "***** Tree not empty after Erase()" << endl;
	}
	// Make sure operations fail on an empty tree
	value_type value;
	err += (tree.Find (keys[0]));
	err += (tree.Remove (keys[0]));
	err += (tree.First ());
	err += (tree.Next ());
	err += (tree.Value (&value));
	err += (tree.Last ());
	err += (tree.Prev ());

	// Sequential insert and partial removal and re-insert
	cout << "Sequential insert and partial removal... " 
	     << *keys.begin() << " to " << keys.back() << endl;
	err += T_Insert (tree, keys, values);
	err += tree.Check ();
	Summarize(cout, tree);

	if (T_Reopen (tree))
		return (err+1);
	if (Debug) tree.Print(cout);

	cout << "Removal of half of keys..." << endl;
	err += T_PartialRemoval (tree, N/2);
	if (Debug) tree.Print(cout);

	if (T_Reopen (tree))
		return (err+1);
	if (Debug) tree.Print(cout);

	cout << "Random removal..." << endl;
	err += T_RandomRemoval (tree);
	err += tree.Check ();

	// Do the reverse test
	reverse (keys.begin(), keys.end());
	cout << "Reverse insert... "
	     << *keys.begin() << " to " << keys.back() << endl;
	err += T_Insert (tree, keys, values);
	err += tree.Check ();
	Summarize(cout, tree);

	if (T_Reopen (tree))
		return (err+1);

	// Test traversal
	err += T_Traversal (tree);

	// Random overwrites, new values
	cout << "Random overwrites... " << endl;
	random_shuffle (keys.begin(), keys.end());
	err += T_Insert (tree, keys, values);
	err += tree.Check ();
	Summarize(cout, tree);

	if (T_Reopen (tree))
		return (err+1);

	// Random removal
	cout << "Random removal... " << endl;
	err += T_RandomRemoval (tree);
	err += tree.Check ();

	// Do the above tests a few times with randomly inserted keys
	for (int i = 0; i < 3; ++i)
	{
		srand (time(0)+(i << (2*i)));
		random_shuffle (keys.begin(), keys.end());

		if (T_Reopen (tree))
			return (err+1);
		cout << "Random insertions... " << i+1 << endl;
		err += T_Insert (tree, keys, values);
		err += tree.Check ();
		Summarize(cout, tree);
		if (T_Reopen (tree))
			return (err+1);
		cout << "Partial removal and re-insertion..." << endl;
		err += T_PartialRemoval (tree, N/2);
		err += tree.Check ();
		cout << "Random removal...... " << i+1 << endl;
		err += T_RandomRemoval (tree);
		err += tree.Check ();
	}
	/*
	 * On the last one check the stats of an emptied tree.
	 */
	Summarize(cout, tree);

	return err;
}


}; // class TreeTest


template <class T>
int
TestTree (T &tree, const char *name, int N)
{
	cout << "-----------------================----------------" << endl;
	int err = 0;
	TreeTest<T> test;
	tree.Erase ();
	cout << "Testing " << name << " tree of order " << tree.Order() 
	     << " with " << N << " members..." << endl;
	err += test.TestTree (tree, N);
	return err;
}



int main (int argc, char *argv[])
{
	int N = 10;
	int order = 3;

	srand (1000);

	// Set the default logger.
	//ofstream lf("tbtree.log");
	//StreamLogger log(std::cerr);
	//Logger::Prototype (log);

	if (argc > 1)
		N = atoi(argv[1]);
	if (argc > 2)
		order = atoi(argv[2]);

	int err = 0;
	// When both number and order given, test that combo once
	// with the compiled test_key type.
	if (argc > 2)
	{
	    {
		default_tree tree(order);
		//TreeTest<default_tree> test;
		//err += test.T_Traversal (tree, 1);
		tree.Check();
		if (Debug) tree.Print(cout);
		err += TestTree (tree, "default", N);
		tree.Destroy();
	    }
	    if (err) cout << "***** ";
	    cout << err << " errors." << endl;
	    exit (err);
	}

	// Perform the tests on trees of various types and orders, 
	// using a given N.
	if (argc <= 1)
		N = 5000;
	BlockFile bf("shartree.bf", 0, BlockFile::BF_CREATE);
	BlockFile ebf("excltree.bf", 0, 
		      BlockFile::BF_CREATE | BlockFile::BF_EXCLUSIVE);
	for (int o = 32; o <= 256; o *= 4)
	{
		{
			// Try memory trees first.
			LongTree ltree(o);
			err += TestTree (ltree, "long", N);
			StringTree stree(o);
			stree.setElementSize (12);
			err += TestTree (stree, "string", N);
			TimeTree ttree(o);
			err += TestTree (ttree, "time", N);
			ltree.Destroy();
			stree.Destroy();
			ttree.Destroy();
		}
		{
			// Now the file trees with our shared blockfile
			LongFileTree lftree(0, bf, o);
			err += TestTree (lftree, "long file", N);
			StringFileTree sftree(0, bf, o);
			sftree.setElementSize (12);
			sftree.setKeySize (12);
			err += TestTree (sftree, "string file", N);
			lftree.Destroy();
			sftree.Destroy();
		}
		{
			// Now file trees with default access
			LongFileTree lftree(0, ebf, o);
			err += TestTree (lftree, "long excl file", N);
			StringFileTree sftree(0, ebf, o);
			sftree.setElementSize (12);
			sftree.setKeySize (12);
			err += TestTree (sftree, "string excl file", N);
			lftree.Destroy();
			sftree.Destroy();
		}
		// Throw in a default file to exercise actually
		// closing and opening the blockfile on reopen()
		unlink ("btree.bf");
	        TimeFileTree tftree (o);
		err += TestTree (tftree, "time default file", N);
		tftree.Destroy();
	}
	
	if (err) cout << "***** ";
	cout << err << " errors." << endl;
	exit (err);
}


