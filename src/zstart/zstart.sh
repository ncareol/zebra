#!/bin/csh -f
#
# This is an attempt at a generalized zeb startup script.
#
# $Id: zstart.sh,v 1.6 1995-04-21 16:27:58 granger Exp $
#
# Here we do basic location of directories, set environment variables,
# and try to hand things off to a project-specific startup file.
#

#
# If this user has a zebra resource file, source it immediately.  That
# way they can override settings for themselves, such as ZEB_TOPDIR,
# ZEB_PROJDIR, and ZEB_CONFIGS.
#
	if ($?HOME) then
		if (-f $HOME/.zebra) then
			source "$HOME/.zebra"
		endif
	endif

#
# The base directory must be right.  We need to either fix things up to
# substitute it during a zeb install, or this script needs to be edited.
#
	if (! $?ZEB_TOPDIR ) setenv ZEB_TOPDIR ##TOPDIRHERE
#
# Tweak some important variables.
#
	set path=($ZEB_TOPDIR/bin $path)

# 
# Add ZEB_TOPDIR/lib/resources directory for X application defaults.  Using
# XFILESEARCHPATH puts these resources very near the bottom of the search 
# heirarchy, so they're easily overridden.
#
	if ( $?XFILESEARCHPATH ) then
		setenv XFILESEARCHPATH \
			${XFILESEARCHPATH}:$ZEB_TOPDIR/lib/resources/%N
	else
		setenv XFILESEARCHPATH $ZEB_TOPDIR/lib/resources/%N
	endif

#
# Look at args
#
	unset projdir
	set dmonly=0
	set dsonly=0
	set execshell=0
	while ($#argv)
		switch ($argv[1])
		    case -preserve:
		    	setenv PRESERVE_ZEB yes
			breaksw
		    case -data*:
			setenv DATA_DIR $argv[2]
			shift
			breaksw
		    case -dm:
			set dmonly=1
			breaksw
		    case -ds:
			set dsonly=1
			breaksw
		    case -shell:
			set execshell=1;
			breaksw
		    default:
		    	set projdir=$argv[1]
			breaksw
		endsw
		shift
	end
#
# Figure out where the project directory is.
#
	if ( ! $?projdir ) then
		if ( -f proj_startup || -f proj_env ) then
			set projdir=`pwd`
		else if ( $?ZEB_PROJDIR ) then
			set projdir="$ZEB_PROJDIR"
		else
			echo -n "Please enter the project directory name: "
			set projdir=$<
		endif
	endif

again:
#
# See if we can find it.
#
	if (-d "$projdir" ) then
		; # nothing
	else if ( -d $ZEB_TOPDIR/$projdir ) then
		set projdir=$ZEB_TOPDIR/$projdir
	else if ( -d $ZEB_TOPDIR/project/$projdir ) then
		set projdir=$ZEB_TOPDIR/project/$projdir
	else
		echo "I can't find project directory" $projdir
		echo -n "Try again: "
		set projdir=$<
		goto again
	endif
	echo 'Project dir is' $projdir
	setenv ZEB_PROJECT $projdir:t
#
# Give the project the chance to augment some settings, supply defaults, 
# or replace some settings.  If it does not set a data directory, zstart
# will prompt the user for one.
#
	cd $projdir
	if (-f proj_env) source proj_env

#
# Make pointers to all of our executables so that somebody can
# override them (such as in proj_env or .zebra) if desired.
#
	if (! $?ZEB_MESSAGE) setenv ZEB_MESSAGE $ZEB_TOPDIR/bin/message
	if (! $?ZEB_EVENTLOGGER) setenv ZEB_EVENTLOGGER \
					 $ZEB_TOPDIR/bin/EventLogger
	if (! $?ZEB_TIMER) setenv ZEB_TIMER $ZEB_TOPDIR/bin/timer
	if (! $?ZEB_DSDAEMON) setenv ZEB_DSDAEMON $ZEB_TOPDIR/bin/dsDaemon
	if (! $?ZEB_DM) setenv ZEB_DM $ZEB_TOPDIR/bin/dm

# 
# Jump to the shell exec if nothing else explicitly requested
#
	if (! $dmonly && ! $dsonly && $execshell) goto start_shell

#
# Data directory.
#
ddir_again:
	if ( ! $dmonly && ! $?DATA_DIR && ! $?DS_DAEMON_HOST ) then
		echo -n "Where is your data directory? "
		set ddir=$<
		if ( ! -d "$ddir" ) then
			echo Directory $ddir does not exist!
			goto ddir_again
		endif
		setenv DATA_DIR $ddir
	endif

#
# Set the color map to something that will hopefully make outlines
# show up.  Test its exit status to see if we could connect to a display.
#
	setenv HOST `uname -n`
	tweakcolor red
	if ( $status != 0 ) then
		echo ' '
		echo 'An error occurred connecting to the X server.  Make sure'
		echo 'the DISPLAY environment variable is set correctly.'
		echo 'For example,'
		echo "   $HOST% setenv DISPLAY server:0.0"
		echo 'where "server" is the name of the machine whose display'
		echo "will be used.  Then run 'xhost $HOST' on the server."
		exit 1
	endif		

#
# If the user told us only to do the dm startup, so be it
#
	if ($dmonly) goto start_dm

#
# Do everything possible to insure that we start clean.  That means either
# stopping a running Zeb or setting ZEB_SOCKET to a unique name. 'mstatus'
# exits with 0 status when message is running, 1 otherwise.  Use the -u
# option to get the name of the user.
#
	set someone=`mstatus -u`
	if ($status == 0) then
		echo "User $someone is already running Zeb.  Enter"
restart_prompt:
		echo "  1) to stop the current Zeb session and start over, or"
		echo "  2) to run an additional, separate session of Zeb, or"
		echo "  3) to start a display manager for this session."
		echo -n "Please enter 1, 2, 3, or [Qq]uit: "
		set ans=$<
		if ( "$ans" =~ [Qq]* ) then
		  exit 1
		else if ("$ans" == "1") then
		  echo "Stopping the current Zeb session."
		else if ("$ans" == "2") then
		  echo "Starting a new Zeb session."
		  if (! $?USER) then
			setenv ZEB_SOCKET /tmp/zeb.socket.$$
		  else
			setenv ZEB_SOCKET /tmp/zeb.$USER.$$
		  endif
		  echo "This Zeb session will use the socket $ZEB_SOCKET"
		  echo "Enter 'setenv ZEB_SOCKET $ZEB_SOCKET' at the C-shell"
		  echo "prompt to run Zeb programs from that shell."
		else if ("$ans" == "3") then
		  echo "Starting a display manager."
		  goto start_dm
		else
		  goto restart_prompt
		endif
	endif
#
# Now try to start clean, whether deliberately killing an existing Zeb
# or just cleaning a leftover socket file
#
	zstop >& /dev/null
	sleep 1
	if ( ! $?ZEB_SOCKET ) then
		rm -f /tmp/fcc.socket
	else
		rm -f $ZEB_SOCKET
	endif
#
# Start core processes.  Message is started in foreground now; we'll wait
# for it to background itself.  Use eval, though, in case someone wants to
# redefine it (such as to a debugger or testcenter) and use an explicit '&'.
#
	echo 'Starting core zeb processes: '
	echo '	message daemon'
	eval $ZEB_MESSAGE

	echo -n '	event logger '
	$ZEB_EVENTLOGGER &
	sleep 1

	echo -n '	timer '
	$ZEB_TIMER &
#
# Now we need to run the per-project startup file.  Things are running so
# this file can actually start processes and zebra clients (but none that
# need the datastore daemon, unless DS_DAEMON_HOST is set).
#
	if ( -f proj_startup ) source proj_startup
#
# Fire off the data store daemon and the display manager.  Daemon only if
# there is not a pointer to a remote host, however.
#
	if ($?DS_DAEMON_HOST) then
		echo '	(DS Daemon running on' $DS_DAEMON_HOST ')'
	else
		$ZEB_DSDAEMON ds.config &
	endif
#
# If they only wanted a baseline datastore running, we quit here
#
	if ( $dsonly && $execshell ) goto start_shell
	if ( $dsonly ) exit 0
	sleep 5

start_dm:
	if ( ! $?DEFAULT_CONFIG ) setenv DEFAULT_CONFIG empty
	if ( ! $?ZEB_DM_CONFIG ) setenv ZEB_DM_CONFIG dm.config
	eval $ZEB_DM $ZEB_DM_CONFIG
#
# Maybe we shut down.
#
	if ( ! $?PRESERVE_ZEB ) zstop
	if ( ! $execshell ) exit 0
#
# Fall through on -shell option
#
start_shell:
	if ( ! $?ZEB_SHELL ) setenv ZEB_SHELL 'csh -f'
	set path=(. $path)
	exec $ZEB_SHELL
