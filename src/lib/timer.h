/* $Id: timer.h,v 1.1 1990-04-18 16:03:31 corbet Exp $ */
/*
 * Timer module protocol requests and responses.
 */

/*
 * Possible client requests.
 */
# define TR_TIME	1	/* What time is it?		*/
# define TR_ABSOLUTE	2	/* Absolute alarm request	*/
# define TR_RELATIVE	3	/* Relative alarm request	*/
# define TR_SET		4	/* Set fake clock timer mode	*/
# define TR_CANCEL	5	/* Cancel all alarm requests	*/
# define TR_STATUS	6	/* Get back status		*/

/*
 * Incremental times are stored as fractions of seconds -- INCFRAC to be
 * exact.
 */
# define INCFRAC 10

/*
 * The name of the timer process.
 */
# define TIMER_PROC_NAME "Timer"

/*
 * The basic request template.
 */
struct tm_req
{
	int	tr_type;
};


/*
 * The basic time request answer.  All responses follow this template.
 */
# define TRR_TIME	100
struct tm_time
{
	int	tm_type;	/* Answer type			*/
	time	tm_time;	/* The current time value	*/
};


/*
 * Alarm requests.
 */
struct tm_abs_alarm_req
{
	int	tr_type;	/* Request type	(== TR_ABSOLUTE)*/
	time	tr_when;	/* When the alarm happens	*/
	int	tr_inc;		/* Increment to next alarm	*/
	int	tr_align;	/* Align for alarm		*/
	int	tr_param;	/* Param to go back with alarm	*/
};

struct tm_rel_alarm_req
{
	int	tr_type;	/* Request type (== TR_RELATIVE)*/
	int	tr_delay;	/* When the alarm happens	*/
	int	tr_inc;		/* Increment to next alarm	*/
	int	tr_align;	/* Align for alarm		*/
	int	tr_param;	/* Param to go back with alarm	*/
};

/*
 * An alarm response.
 */
# define TRR_ALARM	101
struct tm_alarm
{
	int	tm_type;	/* == TRR_ALARM			*/
	time	tm_time;	/* The current time		*/
	int	tm_param;	/* Client-supplied time		*/
};

/*
 * The status response.
 */
# define TRR_STATUS	100
struct tm_status
{
	int	tm_type;	/* Answer type			*/
	time	tm_time;	/* The current time value	*/
	char	tm_status[0];	/* Actual status -- as long as nec.	*/
};

