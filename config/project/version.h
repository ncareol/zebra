/*
 * $Id: version.h,v 1.8 1995-06-29 20:53:19 granger Exp $
 *
 * Include various symbols, compilation, and version info into an object
 * file.  We try to take advantage of ANSI C pre-preprocessors as much as
 * possible, as well as allowing for the use what(1) or ident(1) to identify
 * version information.
 */

#ifndef _zeb_version_h__
#define _zeb_version_h__

#include "config.h"

#if !defined(SABER) && !defined(lint) && !defined(LINT)

#if __STDC__

static char cppsyms[] = "@(#)$Symbols: "
" __STDC__ "
#ifdef _POSIX_SOURCE
" _POSIX_SOURCE "
#endif
#ifdef __GNUC__
" __GNUC__ "
#endif
#ifdef __STRICT_ANSI__
" __STRICT_ANSI__ "
#endif
#ifdef X_NOT_STDC_ENV
" X_NOT_STDC_ENV "
#endif
#ifdef X_NOT_POSIX
" X_NOT_POSIX "
#endif
#ifdef SYSV
" SYSV "
#endif
#ifdef SVR4
" SVR4 "
#endif
#ifdef BSD
" BSD "
#endif
#ifdef SHM
" SHM "
#endif
#ifdef CFG_NC_DCATTS_OVERRIDE
" CFG_NC_DCATTS_OVERRIDE "
#endif
#ifdef CFG_NO_BADVALUES
" CFG_NO_BADVALUES "
#endif
#ifdef CFG_NC_NO_ALT_UNITS
" CFG_NC_NO_ALT_UNITS "
#endif
#ifdef ARM_PROJECT
" ARM_PROJECT "
#endif
#ifdef NEXUS_PROJECT
" NEXUS_PROJECT "
#endif
#ifdef BASEDIR
"BASEDIR:" BASEDIR " "
#endif
#ifdef BINDIR
"BINDIR:" BINDIR " "
#endif
#ifdef LIBDIR
"LIBDIR:" LIBDIR " "
#endif
#ifdef AutoBuild
" AutoBuild "
#endif
" $";
#ifdef __GNUC__
static const char *use_cppsyms = (0, use_cppsyms, cppsyms);
#endif
#else
static char cppsyms[] = "@(#)$Symbols: __STDC__ not defined$";
#endif /* __STDC__ */

#endif /* !SABER && !lint */

/*-----------------------------------------------------------------------
 * Version management macros
 */

/* Note that any necessary semi-colons are already included and should
 * not be added when invoked in code.  Prepending "@(#)" allows version
 * info to be found by both ident(1) (RCS) and what(1) (SCCS).
 * Other RCS info, besides the Id, can be generated by using
 * differently named macros, which define different static char symbols.
 * The compile date and time are only included with RCSID.
 */

#if defined(lint) || defined(LINT) || defined(SABER)

#define RCSID(id) 
#define RCSAUTHOR(id) 
#define RCSSTATE(id) 

#else
#if __STDC__

#ifdef __GNUC__
#define RCSID(id) \
static const char i_sccsid[4] = { '@', '(', '#', ')' }; \
static const char rcs_id[] = "@(#)" id ;			  \
static const char compileid[] = 			  \
	"@(#)" "$Compiled: " __FILE__ " on " __DATE__ " at " __TIME__ " $"; \
static const char *use_i_sccsid = (0, use_i_sccsid, i_sccsid); \
static const char *use_rcs_id = (0, use_rcs_id, rcs_id); \
static const char *use_compileid = (0, use_compileid, compileid);

#define RCSAUTHOR(id) \
static const char rcs_author[] = "@(#)" id ; \
static const char *use_rcs_author = (0, use_rcs_author, rcs_author);

#define RCSSTATE(id) \
static const char rcs_state[] = "@(#)" id ; \
static const char *use_rcs_state = (0, use_rcs_state, rcs_state);

#else /* !__GNUC__ */

#define RCSID(id) \
static const char i_sccsid[4] = { '@', '(', '#', ')' }; \
static const char rcs_id[] = "@(#)" id ;			  \
static const char compileid[] = 			  \
	"@(#)" "$Compiled: " __FILE__ " on " __DATE__ " at " __TIME__ " $";

#define RCSAUTHOR(id) \
static const char rcs_author[] = "@(#)" id ;


#define RCSSTATE(id) \
static const char rcs_state[] = "@(#)" id ;

#endif /* __GNUC__ */

#else /* not lint and not __stdc__ */
/*
 * These defs are not as complete as above. And the 'what' flags may be
 * lost on optimization or not left preceding the RCS string.
 */

#define RCSID(id) \
static const char i_sccsid[4] = { '@', '(', '#', ')' }; \
static const char rcs_id[] = id ;

#define RCSSTATE(id) \
static const char s_sccsid[4] = { '@', '(', '#', ')' }; \
static const char rcs_state[] = id ;

#define RCSAUTHOR(id) \
static const char a_sccsid[4] = { '@', '(', '#', ')' }; \
static const char rcs_author[] = id ;

#endif /* __STDC__ */
#endif /* lint */


#if !defined(lint) && !defined(LINT) && !defined(SABER)

#if __STDC__
static const char V_sccsid[4] = { '@', '(', '#', ')' };
static const char V_rcs_id[] = "@(#)$Id: version.h,v 1.8 1995-06-29 20:53:19 granger Exp $";
static const char V_compileid[] = 
	"@(#)" "$Included: " __FILE__ " on " __DATE__ " at " __TIME__ " $";
#ifdef __GNUC__
static const char *use_V_sccsid = (0, use_V_sccsid, V_sccsid);
static const char *use_V_rcs_id = (0, use_V_rcs_id, V_rcs_id);
static const char *use_V_compileid = (0, use_V_compileid, V_compileid);
#endif
#endif /* __STDC__ */

/*
 * Hand edit this line until it can be done automatically.  One possibility
 * is using the RCS state field, but even that requires running a command
 * manually.  Perhaps an explicit version tag script which tags with CVS as
 * well as updating the ChangeLog and this file.
 */
static const char zeb_version_id1[] = 
"@(#)$ZebraVersion: 4.2-alpha $";
static const char zeb_version_id2[] = 
"@(#)$ZebraVersion: Research Data Program, NCAR $";
static const char zeb_version_id3[] = 
"@(#)$Copyright: University Corporation for Atmospheric Research, 1994 $";

#if __GNUC__
static const char *use_zeb_version_id1 = 
	(0, use_zeb_version_id1, zeb_version_id1);
static const char *use_zeb_version_id2 = 
	(0, use_zeb_version_id2, zeb_version_id2);
static const char *use_zeb_version_id3 = 
	(0, use_zeb_version_id3, zeb_version_id3);
#endif /* __GNUC__ */

#endif /* lint, LINT, and SABER */

#endif /* !_zeb_version_h__ */
