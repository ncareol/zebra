#include <stdio.h>
#include <fcntl.h>
#define NROWS	1100
#define NCOLS	1100
#define NPTS	5
#define NX1	512
#define NY1	512
extern unsigned char IMAGE[NROWS][NCOLS];
unsigned char small[NY1][NX1];
int file = 0;

#include "lcw.h"

DumpImage(dtg)
struct lcw *dtg;	/* Line Control Word */
	{
	int i, j;
	int k, m;
	int element, line;	/* coordinates of top left  corner of subwindow  */

	/* average the image down to 1/5 resolution */

#define HALF	2
	long int sum;
	int jlow, jhi, ilow, ihi;
	float scale;
	int file;
	int value;
	int left, right, bottom, top;


	MakeGrid();	/* get navigation info */
	Navigate(10., 100., &element, &line);
	/* for each box in the small array, copy a sub window */

	for( m = 0; m < NY1; m++)
		for( k = 0; k < NX1; k++)
			small[m][k] = IMAGE[line + m][element + k];

#ifdef graphics
	left = element + 0;
	right = left + NX1 - 1;
	top  = 1100 - line;
	bottom = top - (NY1 -1);
	mgiimage(small, left, bottom, right, top);  
#endif
	DumpArray(dtg, line, element);
	return(0);
	}
int MakeImage(IRorVIS, Year, Month, Day, GMTime)
char IRorVIS;	/* either a "V" or "I" */
char *Year, *Month, *Day, *GMTime;	/* (input) NOT null terminated strings! */
	{
	char
		*Type;
	static char 
		*IR  = "INFRARED", 
		*VIS = " VISIBLE";
/* IR data needs to be modified */
	int i, j;
	char string[40];
	static char cmin[4];
	int minutes;
	
/*	transfer(); */
/*	filter(); */
/*	mgiimage(SCREEN, 0, 0, NXPIX -1, NYPIX -1); */
#ifdef flipir
	/* reverse the IR values, to make a positive image */
	if( IRorVIS == 'I') 
		flipir(IMAGE, sizeof(IMAGE)); 
#endif

/*	mgiimage(IMAGE, 0, 0, NCOLS - 1, NROWS - 1); */
/* do a zoomed copy from buffer 2 to 1 */

/*	mgifbfbzm(1, 2, 0,0, 2,2, 256 + 64,256,512 + 64,512,4); */
#ifdef graphics
	mgifb(1,1);	/* show and modify buffer 1*/ 
	mgipln(255);
	mgisyncrb(1);
	mgihue(0);
	/* black out a box for time and date */
	mgibox(450, 850, 800, 890);
	if( IRorVIS == 'I')
		{
		Type = IR;

		}
	else
		Type = VIS;
	/* convert the time to hours, round off to nearest ten  minutes, and
	use that time in the file name */
	sprintf(string, "%s %.2s/%.2s/%.4s %.4s UTC", Type, Month, Day, Year, GMTime);
	mgihue(255);
	mgigfs(460, 860, 0, string);
#endif
	return(0);
	}
#ifdef old
transfer()
	{
	register int i, j;
	int maxx, maxy;
	maxx = 1100;	/* mininum of screen width and image width */
	maxy = 1100 / 2;	/* minimum of screen height and image height */
	for(i = 0; i < maxy; i++)
		for(j = 0; j < maxx; j++)
			SCREEN[i][j] = IMAGE[2 * i + 1][j];
	return(0);
	}
#endif
flipir(data, n)
register int n;
char *data;
	{
	register char *c;
	register int temp;
	c = data;
	while(n--)
		{
		temp = abs(*c - 255);
		*c++ = temp;
		}
	return(0);
	}

#ifdef old
filter()

	{
	int i,j;
	/* flip values of every other row */
	for( i = 0; i < NYPIX; i += 2)
		for(j = 0; j < NXPIX; j++)
		
		{
		SCREEN[i][j] = abs(SCREEN[i][j] - 255);
		}
	return(0);
	}
#endif
StartGraphics()
	{
	int i, value;
#ifdef graphics
	mgiasngp(0,0);
	mgiclearpln(0,-1,0);
	mgifetchgf(1,"7x9_bold");
	mgigf(1);
	/* load a straight black and white gray scale */
	for( i = 0; i < 255; i++)
		{
		value =( i << 16 ) | (i << 8) | i;
		
		mgicm(255 - i, value);
		}
	value = 0xffffff;
	mgicm(255, value);
#endif
	}
quit()
	{
#ifdef graphics
	mgideagp();
#endif
	close(file);
	exit(0);
	}
int startup = 1;
DumpArray(dtg, line, element)
struct lcw *dtg;
int line, element;
	{
	float time;
	char message[1024];
	static char imonth[3], iday[3], itime[5];
	int month, day, jday, hour, minutes,
		newtime, newhour, newmin;
	static char filename[1024], ctime[4], chour[4], cmin[4];
	bzero(filename, sizeof(filename));
	strncpy(imonth, dtg->Month, 2);
	strncpy(iday, dtg->Day, 2);
	strncpy(itime, dtg->GMTime, 4);
	jday = (atoi(imonth) - 1) * 31 + atoi(iday);
	strncpy(chour, dtg->GMTime, 2);
	hour = atoi(chour);
	strncpy(cmin, &dtg->GMTime[2], 2);
	minutes = atoi( cmin);	
	time = hour * 60 + minutes;
	newtime = (int)(time / 10. + 0.5) * 10;
	newhour = newtime / 60;
	newmin = newtime % 60;
	sprintf(filename, "IR%3.3d.%2.2d%2.2d", jday, newhour, newmin);

	if( file > 0)
		close(file);	/* make sure any previous file is closed */
	file = open(filename, O_WRONLY | O_CREAT, 0777);
	if( file < 1)
		{
		sprintf(message,"File %s not open", filename);
		perror(message);
		return(1);
		}
	write(file, dtg, sizeof(struct lcw));
	write(file, small, sizeof(small));
	/* write the line and element of the top left corner */
	write(file, &line, sizeof(int));
	write(file, &element, sizeof(int));
	return(0);
	}
