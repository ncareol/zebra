/* $Id: ingest.c,v 1.18 1999-03-01 02:03:46 burghart Exp $
 *
 * ingest.c --- A common ingest interface and support routines for 
 *		Zeb ingest modules
 *
 * In general, it defines the IngestLog() function which allows its
 * messages to be directed to stdout and/or to the EventLogger.
 * stdou message can be screened with the command line option -log.
 * Other command line options allow disabling the DataStore and message
 * sending for testing and running in 'dryrun' mode.  Calls to
 * ds_Store and ds_LookupPlatform are macro-defined in ingest.h
 * to point to ingest.c's internal implementations.  These implementations
 * flag ds_Store calls and failures, or ignores the calls when
 * DryRun mode is in effect.  See IngestUsage for the full list
 * of ingest options.
 *
 * An application using the ingest package should do the following:
 *
 * #include "ingest.h"
 * Parse the ingest cmd-line options with IngestParseOptions().  Call
 * IngestInitialize(), which intializes the message and DataStore unless
 * these have been disabled by the application or command-line options.
 * Call IngestUsage() from the application's usage() function to show the
 * user what the ingest options are.  Alternatively, if the application has
 * no usage() function, just pass IngestUsage() to IngestParseOptions
 * as the usage function.  Lastly, use IngestLog() rather than
 * msg_ELog() to log messages with the EventLogger.
 *
 * IngestInitialize() also installs a default message handler which
 * recognizes MH_SHUTDOWN calls.  This handler can be overridden by
 * defining your own protocol handlers with the message library functions.
 */

#include <string.h>
#include <stdio.h>
#include "ingest.h"

/*
 * Undefine the macros defined in ingest.h that we don't need here
 */
#undef ds_Store
#undef ds_StoreBlocks
#undef ds_LookupPlatform
/* #undef ds_DeleteData */

#ifndef lint
MAKE_RCSID("$Id: ingest.c,v 1.18 1999-03-01 02:03:46 burghart Exp $")
#endif


/*
 * Public Prototypes:
 */
zbool _Ingest_ds_Store FP((DataChunk *dc, int newfile, 
			  dsDetail *details, int ndetail));
zbool _Ingest_ds_StoreBlocks FP((DataChunk *dc, int newfile, 
				dsDetail *details, int ndetail));
PlatformId _Ingest_ds_LookupPlatform FP((char *name));
void _Ingest_ds_DeleteData FP((PlatformId platform, int leave));
void IngestUsage ();
void IngestRemoveOptions FP((int *argc, char *argv[], int i, int n));
void IngestParseOptions FP((int *argc, char *argv[], 
			    void (*usage)(/* char *prog */)));
void IngestInitialize FP((char *name));

/*
 * Private prototypes:
 */
static int	IngestMsgHandler FP((struct message *));


/* --------------------------------------------------------------------
 * Global variables :: IngestLogFlags declared external in ingest.h  */

int IngestLogFlags = EF_EMERGENCY;
				/* Specifies what types of messages are
				 * written to the terminal */
short NoEventLogger = 0;	/* True when -noel flag set, meaning
				 * don't try to send messages to ELogger */
short NoDataStore = 0;
short NoMessageHandler = 0;
short DryRun = 0;		/* -dryrun flag, don't connect to message
				 * handler or send to DataStore,
				 * ==> NoEventLogger */
char *IngestName = "UnNamed!";	/* Message name of ingest module */
short ShowIngestName = 0;	/* Show ingest name in local log messages */
short DumpDataChunks = 0;	/* Dump data chunks as ds_Store'd */

/*
 * Private variables
 */
static short Standalone = 0;	/* Run in ds standalone mode */
static char *DataDir = NULL;	/* Default datadir in standalone mode */

/* ------------------------------------------------------------------*/

static int
IngestMsgHandler (msg)
struct message *msg;
/*
 * Deal with IngestMsgHandler messages, especially MH_SHUTDOWN
 */
{
	struct mh_template *mh = (struct mh_template *)msg->m_data;

	IngestLog(EF_DEBUG,
	   "msg handling defaulting to IngestMsgHandler(proto %d)",
	   msg->m_proto);
	switch (msg->m_proto)
	{
	   case MT_TIMER:
	   	tl_DispatchEvent ((struct tm_time *) msg->m_data);
		break;
	   case MT_MESSAGE:
		switch(mh->mh_type)
		{
		   case MH_SHUTDOWN:
			IngestLog(EF_EMERGENCY,
				"Shutdown message received");
			exit(0);
			break;
		   default:
			IngestLog(EF_PROBLEM,
			   "Message handler message %d not handled",
			   mh->mh_type);
		}
		break;
	   default:
		IngestLog(EF_PROBLEM,
			"Message protocol %d not handled",
			msg->m_proto);

	}
	return (0);
}


void
IngestUsage()
{
   const char *spc = " ";

   fprintf(stderr,"Ingest Options:\n");
   fprintf(stderr,"   %-20s Dump data chunks when stored\n","-blow,-chunks");
   fprintf(stderr,"   %-20s Don't send to EventLogger\n","-noel");
   fprintf(stderr,"   %-20s Don't connect to other processes, ",
		  "-dryrun,-dry,-test");
   fprintf(stderr,"e.g. the DataStore\n");
   fprintf(stderr,"   %-20s Set the messages which get ",
	          "-log all|e|p|c|d|i|v");
   fprintf(stderr,"echoed to the terminal\n");
   fprintf(stderr,"   %-20s The default is emergencies only.\n",spc);
   fprintf(stderr,"   %-20s    all: all messages; same as -log epcdiv\n",spc);
   fprintf(stderr,"   %-20s    e:   emergencies\n",spc);
   fprintf(stderr,"   %-20s    p:   problems\n",spc);
   fprintf(stderr,"   %-20s    c:   clients\n",spc);
   fprintf(stderr,"   %-20s    d:   debugging\n",spc);
   fprintf(stderr,"   %-20s    i:   information\n",spc);
   fprintf(stderr,"   %-20s    v:   development debugging\n",spc);
   fprintf(stderr,
	   "   %-20s Print the program name in log messages to the terminal\n",
	   "-name");
   fprintf(stderr,"   %-20s Show this help information\n","-help, -h");
   fprintf(stderr,"   %-20s Write files as standalone (env DS_STANDALONE)\n",
	   "-standalone, -s");
   fprintf(stderr,"   %-20s Standalone data directory (env DS_DATA_DIR)\n",
	   "-data <dir>");
}


void
IngestParseOptions(argc, argv, usage)
	int *argc;
	char *argv[];
	void (*usage)(/* char *prog_name */);
{
	int i;
	int nargs;
	char *arg;

	i = 1;
	while (i < *argc)
	{
		nargs = 1;

		if (streq(argv[i],"-help") ||
		    streq(argv[i],"-h"))
		{
		   if (usage)
		   	usage(argv[0]);
		   exit(0);
		}
		else if (streq(argv[i],"-log"))
		{
		   nargs = 2;
		   arg = argv[i+1];
		   if (! arg)
		   {
			   fprintf(stderr,"-log requires argument\n");
			   usage (argv[0]);
			   exit (1);
		   }
		   else if (streq(arg,"all"))
		      IngestLogFlags = 0xff | EF_DEVELOP;
		   else
		   {
      		      while ((*arg))
      		      {
         		 switch(*arg)
          		 {
			    case 'e':
			       IngestLogFlags |= EF_EMERGENCY;
			       break;
			    case 'c':
			       IngestLogFlags |= EF_CLIENT;
			       break;
    			    case 'd':
       			       IngestLogFlags |= EF_DEBUG;
			       break;
			    case 'p':
			       IngestLogFlags |= EF_PROBLEM;
			       break;
			    case 'i':
			       IngestLogFlags |= EF_INFO;
			       break;
			    case 'v':
			       IngestLogFlags |= EF_DEVELOP;
			       break;
			    default:
			       fprintf(stderr,"Invalid log flag: %c\n",*arg);
         		 }
			 ++arg;
      		      }
		   }
		}
		else if (streq(argv[i],"-data"))
		{
		   nargs = 2;
		   arg = argv[i+1];
		   if (! arg)
		   {
			   fprintf(stderr,"-data requires argument\n");
			   usage (argv[0]);
			   exit (1);
		   }
		   DataDir = arg;
		}
		else if (streq(argv[i],"-noel"))
		{
			SetNoEventLogger();
		}
		else if (streq(argv[i],"-chunks") ||
			 streq(argv[i],"-blow"))
		{
			DumpDataChunks = 1;
		}
		else if (streq(argv[i],"-dryrun") ||
			 streq(argv[i],"-dry") ||
			 streq(argv[i],"-test"))
		{
			SetDryRun();
		}
		else if (streq(argv[i],"-standalone") ||
			 streq(argv[i],"-s"))
		{
			Standalone = 1;
		}
		else if (streq(argv[i],"-name"))
		{
			ShowIngestName = 1;
		}
		else
		{
		   ++i;
		   continue;
		}

		IngestRemoveOptions(argc, argv, i, nargs);
	}
}


void
IngestRemoveOptions(argc, argv, i, n)
	int *argc;
	char *argv[];
	int i;		/* position to start removing args from */
	int n;		/* number of args to remove */
{
	int j;

	(*argc) -= n;
	for (j = i; j < *argc; ++j) 
	   argv[j] = argv[j+n];

}



void
IngestInitialize(name)
	char *name;		/* Message name of this ingest module */
{

	IngestName = (char *)malloc(strlen(name)+1);
	strcpy (IngestName, name);

	if (getenv ("DS_STANDALONE") != NULL)
		Standalone = 1;
	if (Standalone)
	{
		msg_connect (NULL, (ShowIngestName ? IngestName : ""));
		ds_Standalone ();
		fprintf(stderr,"Running standalone.  ");
	}
	else if (!DryRun)
	{
		if (! msg_connect (IngestMsgHandler, IngestName))
		{
			SetNoMessageHandler();
			IngestLog(EF_EMERGENCY, 
			   "Error: msg_connect() failed.");
			exit(1);
		}
		if (! ds_Initialize ())
		{
			SetNoDataStore();
			IngestLog(EF_EMERGENCY,
			   "Error: ds_Initialize() failed.");
			exit(1);
		}
	}
	else
	{
		msg_connect (NULL, (ShowIngestName ? IngestName : ""));
		SetNoDataStore();
		fprintf(stderr,"Running in DryRun mode.  ");
		fprintf(stderr,"No connections or data writes attempted.\n");
	}
	msg_ELPrintMask (IngestLogFlags);
}


PlatformId 
_Ingest_ds_LookupPlatform (name)
	char *name;
{
	static short called_once = 0;

	if (NoDataStore)
	{
		if (called_once) return(0);
		++called_once;
		IngestLog(EF_INFO,
		   "DryRun: Calls to ds_LookupPlatform will return 0");
		return(0);
	}
	else
		return(ds_LookupPlatform(name));
}


zbool 
_Ingest_ds_Store (dc, newfile, details, ndetail)
	DataChunk *dc;
	zbool newfile;
	dsDetail *details;
	int ndetail;
{
	zbool ret;
	static short called_once = 0;

	if (DumpDataChunks)
		dc_DumpDC(dc);
	if (NoDataStore)
	{
		if (!called_once)
		{
			++called_once;
			IngestLog(EF_INFO,
		   	   "DryRun: Calls to DataStore being ignored");
		}
		ret = TRUE;
	}
	else
	{
		IngestLog(EF_DEBUG,
		   "Sending data to the DataStore...");
		ret = ds_Store(dc, newfile, details, ndetail);
		if (!ret)
			IngestLog(EF_PROBLEM,
			   "ds_Store() failed.");
		else
			IngestLog(EF_DEBUG,
			   "ds_Store() successful.");
	}
	return(ret);
}



#ifdef notdef
zbool 
_Ingest_ds_StoreBlocks (dc, newfile, details, ndetail)
	DataChunk *dc;
	zbool newfile;
	dsDetail *details;
	int ndetail;
{
	zbool ret;
	static short called_once = 0;

	if (DumpDataChunks)
		dc_DumpDC(dc);
	if (NoDataStore)
	{
		if (!called_once)
		{
			++called_once;
			IngestLog(EF_INFO,
		   	   "DryRun: Calls to DataStore being ignored");
		}
		ret = TRUE;
	}
	else
	{
		IngestLog(EF_DEBUG,
		   "Sending data to the DataStore...");
		ret = ds_StoreBlocks(dc, newfile, details, ndetail);
		if (!ret)
			IngestLog(EF_PROBLEM,
			   "ds_StoreBlocks() failed.");
		else
			IngestLog(EF_DEBUG,
			   "ds_StoreBlocks() successful.");
	}
	return(ret);
}
#endif /* notdef */


# ifdef notdef
void
_Ingest_ds_DeleteData(platform, leave)
	PlatformId platform;
	int leave;	/* seconds of data to leave */
{
	static short called_once = 0;
	if (NoDataStore)
	{
		if (!called_once) 
		{
			++called_once;
			IngestLog(EF_INFO, 
		   	   "DryRun: Calls to ds_DeleteData being ignored");
		}
	}
	else
	{
		ds_DeleteData(platform, leave);
	}
	return;
}
# endif
