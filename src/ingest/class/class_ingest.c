/*
 * Ingest CLASS data into the system.
 *
 * Usage:
 *	class_ingest file f1 f2 ... fn
 *
 */
# include <ctype.h>
# include <ui_error.h>
# include "../include/defs.h"
# include "../include/message.h"
# include "../include/timer.h"
# include "../include/DataStore.h"
# include "fields.h"

static char *rcsid = "$Id: class_ingest.c,v 2.0 1991-07-18 23:20:21 corbet Exp $";

# ifdef __STDC__
	static int	incoming (struct message *);
	static void	Times (void);
	static void	Locations (void);
# else
	static int	incoming ();
	static void	Times ();
	static void	Locations ();
# endif

/*
 * Our data object
 */
DataObject Dobj;

# define SND	"snd"
# define BUFLEN	1024
# define BADVAL -9999.0

int 	Npts;
float	Buf[BUFLEN];




main (argc, argv)
int argc;
char **argv;
{
	char	plat[30], *site, *snd_site ();
	int	i, f;
/*
 * Basic arg check.
 */
	if (argc < 3)
	{
		printf ("Usage: class_ingest file fields\n");
		exit (1);
	}
/*
 * Hook into the world.
 */
ERRORCATCH
	usy_init ();
	msg_connect (incoming, "class_ingest");
	ds_Initialize ();
/*
 * Initialize data object stuff we know now
 */
	Dobj.do_org = OrgScalar;
	Dobj.do_badval = BADVAL;
	Dobj.do_flags = 0;
/*
 * Load the sounding file (0 indicates we're loading a CLASS format file)
 */
	snd_load_file (argv[1], 0, SND);
/*
 * Get the site name and figure out the platform
 */
	site = snd_site (SND);

	if (sscanf (site, "FIXED, %s", plat) == 1)
		/* do nothing */;
	else if (strncmp (site, "MOBILE", 6) == 0)
		strcpy (plat, "mobile");
	else
		strcpy (plat, site);
/*
 * Make the platform name lower case
 */
	for (i = 0; i < strlen (site); i++)
		plat[i] = tolower (plat[i]);
/*
 * Set the platform in the data object, making sure it's OK along the way
 */
	if ((Dobj.do_id = ds_LookupPlatform (plat)) == BadPlatform)
		ui_error ("Unknown platform: %s", plat);
/*
 * Get the times and locations
 */
	Times ();
	Locations ();
/*
 * Grab each field
 */
	argc -= 2;
	argv += 2;

	Dobj.do_nfield = argc;

	for (f = 0; f < Dobj.do_nfield; f++)
	{
		Dobj.do_fields[f] = usy_string (argv[f]);
		snd_get_data (SND, Buf, BUFLEN, fd_num (argv[f]), BADVAL);
		Dobj.do_data[f] = (float *) malloc (Npts * sizeof (float));
		memcpy (Dobj.do_data[f], Buf, Npts * sizeof (float));
	}
/*
 * Send everything to the data store
 */
	ds_PutData (&Dobj, TRUE);
ON_ERROR
	exit (1);
ENDCATCH
}



static void
Times ()
/*
 * Get the times of the sounding points
 */
{
	time	start, t, snd_time ();
	int	i, hours, minutes, seconds, delta, neg, snd_get_data ();
/*
 * Get the start time and the time data for the sounding
 */
	start = snd_time (SND);

	Npts = Dobj.do_npoint = snd_get_data (SND, Buf, BUFLEN, 
		fd_num ("time"), BADVAL);
/*
 * Allocate the time array for the data object
 */
	Dobj.do_times = (time *) malloc (Npts * sizeof (time));
/*
 * Convert the sounding times, which are in seconds from sounding launch,
 * into absolute times and put them into the data object
 */
	for (i = 0; i < Npts; i++)
	{
		neg = Buf[i] < 0;
		if (neg)
			Buf[i] = -Buf[i];

		hours = (int)(Buf[i] / 3600);
		minutes = (int)((Buf[i] - 3600 * hours) / 60);
		seconds = (int)(Buf[i] - 3600 * hours - 60 * minutes);
		delta = 10000 * hours + 100 * minutes + seconds;

		t = start;
		
		if (neg)
			pmu_dsub (&t.ds_yymmdd, &t.ds_hhmmss, delta);
		else
			pmu_dadd (&t.ds_yymmdd, &t.ds_hhmmss, delta);

		Dobj.do_times[i] = t;
	}

	Dobj.do_begin = start;
	Dobj.do_end = Dobj.do_times[Npts-1];
}




static void
Locations ()
/*
 * Get the locations of the sounding points
 */
{
	float	snd_s_lat (), snd_s_lon (), snd_s_alt ();
	int	i, snd_get_data ();
/*
 * Put in the site location
 */
	Dobj.do_loc.l_lat = snd_s_lat (SND);
	Dobj.do_loc.l_lon = snd_s_lon (SND);
	Dobj.do_loc.l_alt = 0.001 * snd_s_alt (SND);
/*
 * Allocate the location array for the data object
 */
	Dobj.do_aloc = (Location *) malloc (Npts * sizeof (Location));
/*
 * Get the latitude data
 */
	snd_get_data (SND, Buf, BUFLEN, fd_num ("latitude"), BADVAL);

	for (i = 0; i < Npts; i++)
		Dobj.do_aloc[i].l_lat = Buf[i];
/*
 * Get the longitude data
 */
	snd_get_data (SND, Buf, BUFLEN, fd_num ("longitude"), BADVAL);

	for (i = 0; i < Npts; i++)
		Dobj.do_aloc[i].l_lon = Buf[i];
/*
 * Get the altitude data, converting from m to km
 */
	snd_get_data (SND, Buf, BUFLEN, fd_num ("altitude"), BADVAL);

	for (i = 0; i < Npts; i++)
		Dobj.do_aloc[i].l_alt = 0.001 * Buf[i];
}



		
static int
incoming (msg)
struct message *msg;
/*
 * Deal with incoming messages.
 */
{
	switch (msg->m_proto)
	{
	   case MT_TIMER:
	   	tl_DispatchEvent ((struct tm_time *) msg->m_data);
		break;
	}
	return (0);
}
