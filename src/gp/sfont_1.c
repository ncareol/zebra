/* $Id: sfont_1.c,v 1.1 1990-11-26 11:25:22 corbet Exp $ */
/*
 *  GKL_SF2 - GKS Stroke Font 2
 *
 *  Facility:	GKS Utility Library
 *
 *  Abstract:
 *
 *	This module contains the stroke arrays for the STROKE precision 
 *	text font 2 (single-stroke ASCII character set).
 *
 *  Environment:  Runs at any access mode - AST Reentrant
 *
 *  Author: 		Michael Spratte
 *
 *  Creation Date:	28 September 1984
 *
 *  Modified by:
 *
 *  1-001 - Original
 *
 */ 

/*		Declarations		*/

# ifndef VMS
# define readonly
# endif

/*
 * Include Files:
 */
/*		Stroke Arrays for Font 2	*/


/*  Font Size Entry (Pointed to be character code 0)  */

/*				top	cap	base	bottom	*/
static readonly char size[4] = { 16,	12,	-9,	-16 };

/*  Stroke Tables  */

static readonly char ascii_027[ 24] = { 10, '\027',  -7,   16,	/* degrees */
	   0,  10,
	   0,  13,
	   3,  16,
	   6,  16,
	   9,  13,
	   9,  10,
	   6,   7,
	   3,   7,
	   0,  10,
	-128,   0};
static readonly char ascii_040[  6] = {  1, '\040',  -9,   9,	/*   */
	-128,   0};
static readonly char ascii_041[ 28] = { 12, '\041',  -5,   7,	/* ! */
	   0,  12,
	   0,   0,
	-128,   0,
	   2,  12,
	   2,   0,
	-128,   0,
	   0,  -7,
	   0,  -9,
	   2,  -9,
	   2,  -7,
	   0,  -7,
	-128,   0};
static readonly char ascii_042[ 28] = { 12, '\042',  -8,   8,	/* " */
	  -4,  12,
	  -5,   5,
	-128,   0,
	  -3,  12,
	  -5,   5,
	-128,   0,
	   4,  12,
	   3,   5,
	-128,   0,
	   5,  12,
	   3,   5,
	-128,   0};
static readonly char ascii_043[ 28] = { 12, '\043', -10,  11,	/* # */
	   1,  12,
	  -6, -16,
	-128,   0,
	   7,  12,
	   0, -16,
	-128,   0,
	  -6,   1,
	   8,   1,
	-128,   0,
	  -7,  -5,
	   7,  -5,
	-128,   0};
static readonly char ascii_044[ 36] = { 16, '\044', -12,  12,	/* $ */
	   7,   9,
	   2,  12,
	  -2,  12,
	  -7,   9,
	  -7,   5,
	  -2,   2,
	   5,   0,
	   7,  -2,
	   7,  -7,
	   2,  -9,
	  -2,  -9,
	  -7,  -7,
	-128,   0,
	   0,  14,
	   0, -12,
	-128,   0};
static readonly char ascii_045[ 68] = { 32, '\045', -12,  12,	/* % */
	   9,  12,
	  -9,  -9,
	-128,   0,
	  -4,  12,
	  -2,  10,
	  -2,   8,
	  -3,   6,
	  -5,   5,
	  -7,   5,
	  -9,   7,
	  -9,   9,
	  -8,  11,
	  -6,  12,
	  -4,  12,
	  -2,  11,
	   1,  10,
	   4,  10,
	   7,  11,
	   9,  12,
	-128,   0,
	   5,  -2,
	   3,  -3,
	   2,  -5,
	   2,  -7,
	   4,  -9,
	   6,  -9,
	   8,  -8,
	   9,  -6,
	   9,  -4,
	   7,  -2,
	   5,  -2,
	-128,   0};
static readonly char ascii_046[102] = { 49, '\046', -12,  13,	/* & */
	   9,   4,
	   8,   3,
	   9,   2,
	  10,   3,
	  10,   4,
	   9,   5,
	   8,   5,
	   7,   4,
	   6,   2,
	   4,  -4,
	   3,  -6,
	   1,  -8,
	  -1,  -9,
	  -5,  -9,
	  -8,  -8,
	  -9,  -6,
	  -9,  -3,
	  -8,  -1,
	  -2,   3,
	   0,   5,
	   1,   7,
	   1,   9,
	   0,  11,
	  -2,  12,
	  -4,  11,
	  -5,   9,
	  -5,   7,
	  -4,   4,
	  -2,   1,
	   3,  -6,
	   5,  -8,
	   8,  -9,
	   9,  -9,
	  10,  -8,
	  10,  -7,
	-128,   0,
	  -5,  -9,
	  -7,  -8,
	  -8,  -6,
	  -8,  -3,
	  -7,  -1,
	  -5,   1,
	-128,   0,
	  -5,   7,
	  -4,   5,
	   4,  -6,
	   6,  -8,
	   8,  -9,
	-128,   0};
static readonly char ascii_047[ 20] = {  8, '\047',  -5,   5,	/* ' */
	   0,  10,
	  -1,  11,
	   0,  12,
	   1,  11,
	   1,   9,
	   0,   7,
	  -1,   6,
	-128,   0};
static readonly char ascii_050[ 18] = {  7, '\050',  -7,   9,	/* ( */
	   5,  14,
	   0,   9,
	  -2,   5,
	  -2,  -2,
	   0,  -7,
	   5, -12,
	-128,   0};
static readonly char ascii_051[ 18] = {  7, '\051',  -9,   7,	/* ) */
	  -5,  14,
	   0,   9,
	   2,   5,
	   2,  -2,
	   0,  -7,
	  -5, -12,
	-128,   0};
static readonly char ascii_052[ 22] = {  9, '\052',  -8,   8,	/* * */
	   0,  12,
	   0,   0,
	-128,   0,
	  -5,   9,
	   5,   3,
	-128,   0,
	   5,   9,
	  -5,   3,
	-128,   0};
static readonly char ascii_053[ 16] = {  6, '\053', -14,  14,	/* + */
	   0,   9,
	   0,  -9,
	-128,   0,
	  -9,   0,
	   9,   0,
	-128,   0};
static readonly char ascii_054[ 18] = {  7, '\054',  -5,   7,	/* , */
	   2,  -9,
	   0,  -9,
	   0,  -7,
	   2,  -7,
	   2, -12,
	   0, -16,
	-128,   0};
static readonly char ascii_055[ 10] = {  3, '\055', -14,  14,	/* - */
	  -9,   0,
	   9,   0,
	-128,   0};
static readonly char ascii_056[ 16] = {  6, '\056',  -5,   7,	/* . */
	   0,  -7,
	   0,  -9,
	   2,  -9,
	   2,  -7,
	   0,  -7,
	-128,   0};
static readonly char ascii_057[ 10] = {  3, '\057', -12,  12,	/* / */
	   9,  14,
	  -9, -12,
	-128,   0};
static readonly char ascii_060[ 28] = { 12, '\060', -12,  12,	/* 0 */
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   5,  -7,
	   7,  -2,
	   7,   5,
	   5,   9,
	   0,  12,
	-128,   0};
static readonly char ascii_061[ 12] = {  4, '\061', -12,  12,	/* 1 */
	  -5,   7,
	   0,  12,
	   0,  -9,
	-128,   0};
static readonly char ascii_062[ 22] = {  9, '\062', -12,  12,	/* 2 */
	  -7,   9,
	  -2,  12,
	   2,  12,
	   7,   9,
	   7,   5,
	   5,   0,
	  -7,  -9,
	   7,  -9,
	-128,   0};
static readonly char ascii_063[ 34] = { 15, '\063', -12,  12,	/* 3 */
	  -7,   9,
	  -2,  12,
	   2,  12,
	   7,   9,
	   7,   5,
	   2,   2,
	-128,   0,
	   0,   2,
	   2,   2,
	   7,   0,
	   7,  -7,
	   2,  -9,
	  -2,  -9,
	  -7,  -7,
	-128,   0};
static readonly char ascii_064[ 18] = {  7, '\064', -12,  12,	/* 4 */
	   2,  12,
	   2,  -9,
	-128,   0,
	   2,  12,
	  -9,  -2,
	   9,  -2,
	-128,   0};
static readonly char ascii_065[ 32] = { 14, '\065', -12,  12,	/* 5 */
	  -5,  12,
	  -7,   2,
	  -2,   5,
	   0,   5,
	   5,   2,
	   7,  -2,
	   5,  -7,
	   0,  -9,
	  -2,  -9,
	  -7,  -7,
	-128,   0,
	  -5,  12,
	   5,  12,
	-128,   0};
static readonly char ascii_066[ 32] = { 14, '\066', -12,  12,	/* 6 */
	   5,  12,
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   5,  -7,
	   7,  -2,
	   5,   2,
	   0,   5,
	  -5,   2,
	  -7,  -2,
	-128,   0};
static readonly char ascii_067[ 16] = {  6, '\067', -12,  12,	/* 7 */
	   7,  12,
	  -2,  -9,
	-128,   0,
	  -7,  12,
	   7,  12,
	-128,   0};
static readonly char ascii_070[ 42] = { 19, '\070', -12,  12,	/* 8 */
	  -2,  12,
	  -7,   9,
	  -7,   5,
	  -2,   2,
	   2,   2,
	   7,   5,
	   7,   9,
	   2,  12,
	  -2,  12,
	-128,   0,
	  -2,   2,
	  -7,   0,
	  -7,  -7,
	  -2,  -9,
	   2,  -9,
	   7,  -7,
	   7,   0,
	   2,   2,
	-128,   0};
static readonly char ascii_071[ 32] = { 14, '\071', -12,  12,	/* 9 */
	   7,   5,
	   5,   0,
	   0,  -2,
	  -5,   0,
	  -7,   5,
	  -5,   9,
	   0,  12,
	   5,   9,
	   7,   5,
	   7,  -2,
	   5,  -7,
	   0,  -9,
	  -5,  -9,
	-128,   0};
static readonly char ascii_072[ 28] = { 12, '\072',  -5,   7,	/* : */
	   0,   5,
	   0,   2,
	   2,   2,
	   2,   5,
	   0,   5,
	-128,   0,
	   0,  -7,
	   0,  -9,
	   2,  -9,
	   2,  -7,
	   0,  -7,
	-128,   0};
static readonly char ascii_073[ 30] = { 13, '\073',  -5,   7,	/* ; */
	   0,   5,
	   0,   2,
	   2,   2,
	   2,   5,
	   0,   5,
	-128,   0,
	   2,  -9,
	   0,  -9,
	   0,  -7,
	   2,  -7,
	   2, -12,
	   0, -16,
	-128,   0};
static readonly char ascii_074[ 12] = {  4, '\074', -12,  12,	/* < */
	   8,   9,
	  -8,   0,
	   8,  -9,
	-128,   0};
static readonly char ascii_075[ 16] = {  6, '\075', -14,  14,	/* = */
	  -9,   5,
	   9,   5,
	-128,   0,
	  -9,  -5,
	   9,  -5,
	-128,   0};
static readonly char ascii_076[ 12] = {  4, '\076', -12,  12,	/* > */
	  -8,   9,
	   8,   0,
	  -8,  -9,
	-128,   0};
static readonly char ascii_077[ 38] = { 17, '\077',  -9,  12,	/* ? */
	  -5,   9,
	   0,  12,
	   2,  12,
	   7,   9,
	   7,   5,
	   0,   2,
	   0,   0,
	   2,   0,
	   2,   2,
	   7,   5,
	-128,   0,
	   0,  -7,
	   0,  -9,
	   2,  -9,
	   2,  -7,
	   0,  -7,
	-128,   0};
static readonly char ascii_100[ 62] = { 29, '\100',  -8,   9,	/* @ */
	   3,   2,
	   1,   3,
	  -1,   3,
	  -2,   1,
	  -2,   0,
	  -1,  -2,
	   1,  -2,
	   3,  -1,
	-128,   0,
	   3,   3,
	   3,  -1,
	   4,  -2,
	   6,  -2,
	   7,   0,
	   7,   1,
	   6,   4,
	   4,   6,
	   1,   7,
	   0,   7,
	  -3,   6,
	  -5,   4,
	  -6,   1,
	  -6,   0,
	  -5,  -3,
	  -3,  -5,
	   0,  -6,
	   1,  -6,
	   4,  -5,
	-128,   0};
static readonly char ascii_101[ 22] = {  9, '\101', -12,  12,	/* A */
	   0,  12,
	  -9,  -9,
	-128,   0,
	   0,  12,
	   9,  -9,
	-128,   0,
	  -5,  -2,
	   5,  -2,
	-128,   0};
static readonly char ascii_102[ 36] = { 16, '\102', -12,  12,	/* B */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   2,  12,
	   7,   9,
	   7,   5,
	   2,   2,
	-128,   0,
	  -7,   2,
	   2,   2,
	   7,   0,
	   7,  -7,
	   2,  -9,
	  -7,  -9,
	-128,   0};
static readonly char ascii_103[ 26] = { 11, '\103', -12,  14,	/* C */
	   9,   9,
	   5,  12,
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   5,  -9,
	   9,  -7,
	-128,   0};
static readonly char ascii_104[ 28] = { 12, '\104', -12,  12,	/* D */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   0,  12,
	   5,   9,
	   7,   5,
	   7,  -2,
	   5,  -7,
	   0,  -9,
	  -7,  -9,
	-128,   0};
static readonly char ascii_105[ 28] = { 12, '\105', -12,  12,	/* E */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   7,  12,
	-128,   0,
	  -7,   2,
	   2,   2,
	-128,   0,
	  -7,  -9,
	   7,  -9,
	-128,   0};
static readonly char ascii_106[ 22] = {  9, '\106', -12,   9,	/* F */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   7,  12,
	-128,   0,
	  -7,   2,
	   2,   2,
	-128,   0};
static readonly char ascii_107[ 34] = { 15, '\107', -12,  14,	/* G */
	   9,   9,
	   5,  12,
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   5,  -9,
	   9,  -7,
	   9,   0,
	-128,   0,
	   2,   0,
	   9,   0,
	-128,   0};
static readonly char ascii_110[ 22] = {  9, '\110', -12,  12,	/* H */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	   7,  12,
	   7,  -9,
	-128,   0,
	  -7,   2,
	   7,   2,
	-128,   0};
static readonly char ascii_111[ 10] = {  3, '\111',  -5,   5,	/* I */
	   0,  12,
	   0,  -9,
	-128,   0};
static readonly char ascii_112[ 18] = {  7, '\112',  -9,   7,	/* J */
	   2,  12,
	   2,  -5,
	   0,  -9,
	  -5,  -9,
	  -7,  -5,
	  -7,  -2,
	-128,   0};
static readonly char ascii_113[ 22] = {  9, '\113', -12,  12,	/* K */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	   7,  12,
	  -7,  -2,
	-128,   0,
	  -2,   2,
	   7,  -9,
	-128,   0};
static readonly char ascii_114[ 16] = {  6, '\114', -12,   9,	/* L */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  -9,
	   7,  -9,
	-128,   0};
static readonly char ascii_115[ 28] = { 12, '\115', -14,  14,	/* M */
	  -9,  12,
	  -9,  -9,
	-128,   0,
	  -9,  12,
	   0,  -9,
	-128,   0,
	   9,  12,
	   0,  -9,
	-128,   0,
	   9,  12,
	   9,  -9,
	-128,   0};
static readonly char ascii_116[ 22] = {  9, '\116', -12,  12,	/* N */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   7,  -9,
	-128,   0,
	   7,  12,
	   7,  -9,
	-128,   0};
static readonly char ascii_117[ 32] = { 14, '\117', -12,  14,	/* O */
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   2,  -9,
	   7,  -7,
	   9,  -2,
	   9,   5,
	   7,   9,
	   2,  12,
	   0,  12,
	-128,   0};
static readonly char ascii_120[ 24] = { 10, '\120', -12,  12,	/* P */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   2,  12,
	   7,   9,
	   7,   2,
	   2,   0,
	  -7,   0,
	-128,   0};
static readonly char ascii_121[ 38] = { 17, '\121', -12,  14,	/* Q */
	   0,  12,
	  -5,   9,
	  -7,   5,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   2,  -9,
	   7,  -7,
	   9,  -2,
	   9,   5,
	   7,   9,
	   2,  12,
	   0,  12,
	-128,   0,
	   2,  -5,
	   9, -12,
	-128,   0};
static readonly char ascii_122[ 30] = { 13, '\122', -12,  12,	/* R */
	  -7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   2,  12,
	   7,   9,
	   7,   2,
	   2,   0,
	  -7,   0,
	-128,   0,
	   0,   0,
	   7,  -9,
	-128,   0};
static readonly char ascii_123[ 30] = { 13, '\123', -12,  12,	/* S */
	   7,   9,
	   2,  12,
	  -2,  12,
	  -7,   9,
	  -7,   7,
	  -5,   5,
	   5,   0,
	   7,  -2,
	   7,  -7,
	   2,  -9,
	  -2,  -9,
	  -7,  -7,
	-128,   0};
static readonly char ascii_124[ 16] = {  6, '\124', -12,  12,	/* T */
	   0,  12,
	   0,  -9,
	-128,   0,
	  -9,  12,
	   9,  12,
	-128,   0};
static readonly char ascii_125[ 22] = {  9, '\125', -12,  14,	/* U */
	  -7,  12,
	  -7,  -2,
	  -5,  -7,
	   0,  -9,
	   2,  -9,
	   7,  -7,
	   9,  -2,
	   9,  12,
	-128,   0};
static readonly char ascii_126[ 16] = {  6, '\126', -12,  12,	/* V */
	  -9,  12,
	   0,  -9,
	-128,   0,
	   9,  12,
	   0,  -9,
	-128,   0};
static readonly char ascii_127[ 28] = { 12, '\127', -14,  14,	/* W */
	  -9,  12,
	  -5,  -9,
	-128,   0,
	   0,  12,
	  -5,  -9,
	-128,   0,
	   0,  12,
	   5,  -9,
	-128,   0,
	   9,  12,
	   5,  -9,
	-128,   0};
static readonly char ascii_130[ 16] = {  6, '\130', -12,  12,	/* X */
	  -7,  12,
	   7,  -9,
	-128,   0,
	   7,  12,
	  -7,  -9,
	-128,   0};
static readonly char ascii_131[ 18] = {  7, '\131', -12,  12,	/* Y */
	  -9,  12,
	   0,   2,
	   0,  -9,
	-128,   0,
	   9,  12,
	   0,   2,
	-128,   0};
static readonly char ascii_132[ 22] = {  9, '\132', -12,  12,	/* Z */
	   7,  12,
	  -7,  -9,
	-128,   0,
	  -7,  12,
	   7,  12,
	-128,   0,
	  -7,  -9,
	   7,  -9,
	-128,   0};
static readonly char ascii_133[ 28] = { 12, '\133',  -7,   7,	/* [ */
	  -3,  16,
	  -3, -16,
	-128,   0,
	  -2,  16,
	  -2, -16,
	-128,   0,
	  -3,  16,
	   4,  16,
	-128,   0,
	  -3, -16,
	   4, -16,
	-128,   0};
static readonly char ascii_134[ 10] = {  3, '\134', -33,  33,	/* \ */
	 -33,  33,
	  33, -33,
	-128,   0};
static readonly char ascii_135[ 28] = { 12, '\135',  -7,   7,	/* ] */
	   2,  16,
	   2, -16,
	-128,   0,
	   3,  16,
	   3, -16,
	-128,   0,
	  -4,  16,
	   3,  16,
	-128,   0,
	  -4, -16,
	   3, -16,
	-128,   0};
static readonly char ascii_136[ 16] = {  6, '\136',  -8,   8,	/* ^ */
	   0,  14,
	  -8,   0,
	-128,   0,
	   0,  14,
	   8,   0,
	-128,   0};
static readonly char ascii_137[ 30] = { 13, '\137',  -9,   9,	/* _ */
	  -2,   3,
	  -3,   2,
	  -6,   0,
	  -3,  -2,
	  -2,  -3,
	-128,   0,
	  -3,   2,
	  -5,   0,
	  -3,  -2,
	-128,   0,
	  -5,   0,
	   6,   0,
	-128,   0};
static readonly char ascii_140[ 20] = {  8, '\140',  -5,   5,	/* ` */
	   0,  10,
	   1,  11,
	   0,  12,
	  -1,  11,
	  -1,   9,
	   0,   7,
	   1,   6,
	-128,   0};
static readonly char ascii_141[ 82] = { 39, '\141',  -9,  11,	/* a */
	  -4,   3,
	  -4,   2,
	  -5,   2,
	  -5,   3,
	  -4,   4,
	  -2,   5,
	   2,   5,
	   4,   4,
	   5,   3,
	   6,   1,
	   6,  -6,
	   7,  -8,
	   8,  -9,
	-128,   0,
	   5,   3,
	   5,  -6,
	   6,  -8,
	   8,  -9,
	   9,  -9,
	-128,   0,
	   5,   1,
	   4,   0,
	  -2,  -1,
	  -5,  -2,
	  -6,  -4,
	  -6,  -6,
	  -5,  -8,
	  -2,  -9,
	   1,  -9,
	   3,  -8,
	   5,  -6,
	-128,   0,
	  -2,  -1,
	  -4,  -2,
	  -5,  -4,
	  -5,  -6,
	  -4,  -8,
	  -2,  -9,
	-128,   0};
static readonly char ascii_142[ 70] = { 33, '\142', -11,  10,	/* b */
	  -6,  12,
	  -6,  -9,
	-128,   0,
	  -5,  12,
	  -5,  -9,
	-128,   0,
	  -5,   2,
	  -3,   4,
	  -1,   5,
	   1,   5,
	   4,   4,
	   6,   2,
	   7,  -1,
	   7,  -3,
	   6,  -6,
	   4,  -8,
	   1,  -9,
	  -1,  -9,
	  -3,  -8,
	  -5,  -6,
	-128,   0,
	   1,   5,
	   3,   4,
	   5,   2,
	   6,  -1,
	   6,  -3,
	   5,  -6,
	   3,  -8,
	   1,  -9,
	-128,   0,
	  -9,  12,
	  -5,  12,
	-128,   0};
static readonly char ascii_143[ 60] = { 28, '\143', -10,   9,	/* c */
	   5,   2,
	   4,   1,
	   5,   0,
	   6,   1,
	   6,   2,
	   4,   4,
	   2,   5,
	  -1,   5,
	  -4,   4,
	  -6,   2,
	  -7,  -1,
	  -7,  -3,
	  -6,  -6,
	  -4,  -8,
	  -1,  -9,
	   1,  -9,
	   4,  -8,
	   6,  -6,
	-128,   0,
	  -1,   5,
	  -3,   4,
	  -5,   2,
	  -6,  -1,
	  -6,  -3,
	  -5,  -6,
	  -3,  -8,
	  -1,  -9,
	-128,   0};
static readonly char ascii_144[ 76] = { 36, '\144', -10,  11,	/* d */
	   5,  12,
	   5,  -9,
	-128,   0,
	   6,  12,
	   6,  -9,
	-128,   0,
	   5,   2,
	   3,   4,
	   1,   5,
	  -1,   5,
	  -4,   4,
	  -6,   2,
	  -7,  -1,
	  -7,  -3,
	  -6,  -6,
	  -4,  -8,
	  -1,  -9,
	   1,  -9,
	   3,  -8,
	   5,  -6,
	-128,   0,
	  -1,   5,
	  -3,   4,
	  -5,   2,
	  -6,  -1,
	  -6,  -3,
	  -5,  -6,
	  -3,  -8,
	  -1,  -9,
	-128,   0,
	   2,  12,
	   6,  12,
	-128,   0,
	   5,  -9,
	   9,  -9,
	-128,   0};
static readonly char ascii_145[ 66] = { 31, '\145', -10,   9,	/* e */
	  -6,  -1,
	   6,  -1,
	   6,   1,
	   5,   3,
	   4,   4,
	   2,   5,
	  -1,   5,
	  -4,   4,
	  -6,   2,
	  -7,  -1,
	  -7,  -3,
	  -6,  -6,
	  -4,  -8,
	  -1,  -9,
	   1,  -9,
	   4,  -8,
	   6,  -6,
	-128,   0,
	   5,  -1,
	   5,   2,
	   4,   4,
	-128,   0,
	  -1,   5,
	  -3,   4,
	  -5,   2,
	  -6,  -1,
	  -6,  -3,
	  -5,  -6,
	  -3,  -8,
	  -1,  -9,
	-128,   0};
static readonly char ascii_146[ 48] = { 22, '\146',  -7,   6,	/* f */
	   3,  11,
	   2,  10,
	   3,   9,
	   4,  10,
	   4,  11,
	   3,  12,
	   1,  12,
	  -1,  11,
	  -2,   9,
	  -2,  -9,
	-128,   0,
	   1,  12,
	   0,  11,
	  -1,   9,
	  -1,  -9,
	-128,   0,
	  -5,   5,
	   3,   5,
	-128,   0,
	  -5,  -9,
	   2,  -9,
	-128,   0};
static readonly char ascii_147[124] = { 60, '\147',  -9,  10,	/* g */
	  -1,   5,
	  -3,   4,
	  -4,   3,
	  -5,   1,
	  -5,  -1,
	  -4,  -3,
	  -3,  -4,
	  -1,  -5,
	   1,  -5,
	   3,  -4,
	   4,  -3,
	   5,  -1,
	   5,   1,
	   4,   3,
	   3,   4,
	   1,   5,
	  -1,   5,
	-128,   0,
	  -3,   4,
	  -4,   2,
	  -4,  -2,
	  -3,  -4,
	-128,   0,
	   3,  -4,
	   4,  -2,
	   4,   2,
	   3,   4,
	-128,   0,
	   4,   3,
	   5,   4,
	   7,   5,
	   7,   4,
	   5,   4,
	-128,   0,
	  -4,  -3,
	  -5,  -4,
	  -6,  -6,
	  -6,  -7,
	  -5,  -9,
	  -2, -10,
	   3, -10,
	   6, -11,
	   7, -12,
	-128,   0,
	  -6,  -7,
	  -5,  -8,
	  -2,  -9,
	   3,  -9,
	   6, -10,
	   7, -12,
	   7, -13,
	   6, -15,
	   3, -16,
	  -3, -16,
	  -6, -15,
	  -7, -13,
	  -7, -12,
	  -6, -10,
	  -3,  -9,
	-128,   0};
static readonly char ascii_150[ 60] = { 28, '\150', -11,  11,	/* h */
	  -6,  12,
	  -6,  -9,
	-128,   0,
	  -5,  12,
	  -5,  -9,
	-128,   0,
	  -5,   2,
	  -3,   4,
	   0,   5,
	   2,   5,
	   5,   4,
	   6,   2,
	   6,  -9,
	-128,   0,
	   2,   5,
	   4,   4,
	   5,   2,
	   5,  -9,
	-128,   0,
	  -9,  12,
	  -5,  12,
	-128,   0,
	  -9,  -9,
	  -2,  -9,
	-128,   0,
	   2,  -9,
	   9,  -9,
	-128,   0};
static readonly char ascii_151[ 40] = { 18, '\151',  -5,   6,	/* i */
	   0,  12,
	  -1,  11,
	   0,  10,
	   1,  11,
	   0,  12,
	-128,   0,
	   0,   5,
	   0,  -9,
	-128,   0,
	   1,   5,
	   1,  -9,
	-128,   0,
	  -3,   5,
	   1,   5,
	-128,   0,
	  -3,  -9,
	   4,  -9,
	-128,   0};
static readonly char ascii_152[ 54] = { 25, '\152',  -5,   6,	/* j */
	   1,  12,
	   0,  11,
	   1,  10,
	   2,  11,
	   1,  12,
	-128,   0,
	   2,   5,
	   2, -13,
	   1, -15,
	  -1, -16,
	  -3, -16,
	  -4, -15,
	  -4, -14,
	  -3, -13,
	  -2, -14,
	  -3, -15,
	-128,   0,
	   1,   5,
	   1, -13,
	   0, -15,
	  -1, -16,
	-128,   0,
	  -2,   5,
	   2,   5,
	-128,   0};
static readonly char ascii_153[ 58] = { 27, '\153', -11,  10,	/* k */
	  -6,  12,
	  -6,  -9,
	-128,   0,
	  -5,  12,
	  -5,  -9,
	-128,   0,
	   5,   5,
	  -5,  -5,
	-128,   0,
	   0,  -1,
	   6,  -9,
	-128,   0,
	  -1,  -1,
	   5,  -9,
	-128,   0,
	  -9,  12,
	  -5,  12,
	-128,   0,
	   2,   5,
	   8,   5,
	-128,   0,
	  -9,  -9,
	  -2,  -9,
	-128,   0,
	   2,  -9,
	   8,  -9,
	-128,   0};
static readonly char ascii_154[ 28] = { 12, '\154',  -5,   6,	/* l */
	   0,  12,
	   0,  -9,
	-128,   0,
	   1,  12,
	   1,  -9,
	-128,   0,
	  -3,  12,
	   1,  12,
	-128,   0,
	  -3,  -9,
	   4,  -9,
	-128,   0};
static readonly char ascii_155[ 92] = { 44, '\155', -16,  17,	/* m */
	 -11,   5,
	 -11,  -9,
	-128,   0,
	 -10,   5,
	 -10,  -9,
	-128,   0,
	 -10,   2,
	  -8,   4,
	  -5,   5,
	  -3,   5,
	   0,   4,
	   1,   2,
	   1,  -9,
	-128,   0,
	  -3,   5,
	  -1,   4,
	   0,   2,
	   0,  -9,
	-128,   0,
	   1,   2,
	   3,   4,
	   6,   5,
	   8,   5,
	  11,   4,
	  12,   2,
	  12,  -9,
	-128,   0,
	   8,   5,
	  10,   4,
	  11,   2,
	  11,  -9,
	-128,   0,
	 -14,   5,
	 -10,   5,
	-128,   0,
	 -14,  -9,
	  -7,  -9,
	-128,   0,
	  -3,  -9,
	   4,  -9,
	-128,   0,
	   8,  -9,
	  15,  -9,
	-128,   0};
static readonly char ascii_156[ 60] = { 28, '\156', -11,  11,	/* n */
	  -6,   5,
	  -6,  -9,
	-128,   0,
	  -5,   5,
	  -5,  -9,
	-128,   0,
	  -5,   2,
	  -3,   4,
	   0,   5,
	   2,   5,
	   5,   4,
	   6,   2,
	   6,  -9,
	-128,   0,
	   2,   5,
	   4,   4,
	   5,   2,
	   5,  -9,
	-128,   0,
	  -9,   5,
	  -5,   5,
	-128,   0,
	  -9,  -9,
	  -2,  -9,
	-128,   0,
	   2,  -9,
	   9,  -9,
	-128,   0};
static readonly char ascii_157[ 76] = { 36, '\157', -10,  10,	/* o */
	  -1,   5,
	  -4,   4,
	  -6,   2,
	  -7,  -1,
	  -7,  -3,
	  -6,  -6,
	  -4,  -8,
	  -1,  -9,
	   1,  -9,
	   4,  -8,
	   6,  -6,
	   7,  -3,
	   7,  -1,
	   6,   2,
	   4,   4,
	   1,   5,
	  -1,   5,
	-128,   0,
	  -1,   5,
	  -3,   4,
	  -5,   2,
	  -6,  -1,
	  -6,  -3,
	  -5,  -6,
	  -3,  -8,
	  -1,  -9,
	-128,   0,
	   1,  -9,
	   3,  -8,
	   5,  -6,
	   6,  -3,
	   6,  -1,
	   5,   2,
	   3,   4,
	   1,   5,
	-128,   0};
static readonly char ascii_160[ 76] = { 36, '\160', -11,  10,	/* p */
	  -6,   5,
	  -6, -16,
	-128,   0,
	  -5,   5,
	  -5, -16,
	-128,   0,
	  -5,   2,
	  -3,   4,
	  -1,   5,
	   1,   5,
	   4,   4,
	   6,   2,
	   7,  -1,
	   7,  -3,
	   6,  -6,
	   4,  -8,
	   1,  -9,
	  -1,  -9,
	  -3,  -8,
	  -5,  -6,
	-128,   0,
	   1,   5,
	   3,   4,
	   5,   2,
	   6,  -1,
	   6,  -3,
	   5,  -6,
	   3,  -8,
	   1,  -9,
	-128,   0,
	  -9,   5,
	  -5,   5,
	-128,   0,
	  -9, -16,
	  -2, -16,
	-128,   0};
static readonly char ascii_161[ 70] = { 33, '\161', -10,  10,	/* q */
	   5,   5,
	   5, -16,
	-128,   0,
	   6,   5,
	   6, -16,
	-128,   0,
	   5,   2,
	   3,   4,
	   1,   5,
	  -1,   5,
	  -4,   4,
	  -6,   2,
	  -7,  -1,
	  -7,  -3,
	  -6,  -6,
	  -4,  -8,
	  -1,  -9,
	   1,  -9,
	   3,  -8,
	   5,  -6,
	-128,   0,
	  -1,   5,
	  -3,   4,
	  -5,   2,
	  -6,  -1,
	  -6,  -3,
	  -5,  -6,
	  -3,  -8,
	  -1,  -9,
	-128,   0,
	   2, -16,
	   9, -16,
	-128,   0};
static readonly char ascii_162[ 50] = { 23, '\162',  -9,   8,	/* r */
	  -4,   5,
	  -4,  -9,
	-128,   0,
	  -3,   5,
	  -3,  -9,
	-128,   0,
	  -3,  -1,
	  -2,   2,
	   0,   4,
	   2,   5,
	   5,   5,
	   6,   4,
	   6,   3,
	   5,   2,
	   4,   3,
	   5,   4,
	-128,   0,
	  -7,   5,
	  -3,   5,
	-128,   0,
	  -7,  -9,
	   0,  -9,
	-128,   0};
static readonly char ascii_163[ 68] = { 32, '\163',  -8,   9,	/* s */
	   5,   3,
	   6,   5,
	   6,   1,
	   5,   3,
	   4,   4,
	   2,   5,
	  -2,   5,
	  -4,   4,
	  -5,   3,
	  -5,   1,
	  -4,   0,
	  -2,  -1,
	   3,  -3,
	   5,  -4,
	   6,  -5,
	-128,   0,
	  -5,   2,
	  -4,   1,
	  -2,   0,
	   3,  -2,
	   5,  -3,
	   6,  -4,
	   6,  -7,
	   5,  -8,
	   3,  -9,
	  -1,  -9,
	  -3,  -8,
	  -4,  -7,
	  -5,  -5,
	  -5,  -9,
	  -4,  -7,
	-128,   0};
static readonly char ascii_164[ 36] = { 16, '\164',  -7,   8,	/* t */
	  -2,  12,
	  -2,  -5,
	  -1,  -8,
	   1,  -9,
	   3,  -9,
	   5,  -8,
	   6,  -6,
	-128,   0,
	  -1,  12,
	  -1,  -5,
	   0,  -8,
	   1,  -9,
	-128,   0,
	  -5,   5,
	   3,   5,
	-128,   0};
static readonly char ascii_165[ 60] = { 28, '\165', -11,  11,	/* u */
	  -6,   5,
	  -6,  -6,
	  -5,  -8,
	  -2,  -9,
	   0,  -9,
	   3,  -8,
	   5,  -6,
	-128,   0,
	  -5,   5,
	  -5,  -6,
	  -4,  -8,
	  -2,  -9,
	-128,   0,
	   5,   5,
	   5,  -9,
	-128,   0,
	   6,   5,
	   6,  -9,
	-128,   0,
	  -9,   5,
	  -5,   5,
	-128,   0,
	   2,   5,
	   6,   5,
	-128,   0,
	   5,  -9,
	   9,  -9,
	-128,   0};
static readonly char ascii_166[ 34] = { 15, '\166',  -9,   9,	/* v */
	  -6,   5,
	   0,  -9,
	-128,   0,
	  -5,   5,
	   0,  -7,
	-128,   0,
	   6,   5,
	   0,  -9,
	-128,   0,
	  -8,   5,
	  -2,   5,
	-128,   0,
	   2,   5,
	   8,   5,
	-128,   0};
static readonly char ascii_167[ 52] = { 24, '\167', -12,  12,	/* w */
	  -8,   5,
	  -4,  -9,
	-128,   0,
	  -7,   5,
	  -4,  -6,
	-128,   0,
	   0,   5,
	  -4,  -9,
	-128,   0,
	   0,   5,
	   4,  -9,
	-128,   0,
	   1,   5,
	   4,  -6,
	-128,   0,
	   8,   5,
	   4,  -9,
	-128,   0,
	 -11,   5,
	  -4,   5,
	-128,   0,
	   5,   5,
	  11,   5,
	-128,   0};
static readonly char ascii_170[ 46] = { 21, '\170', -10,  10,	/* x */
	  -6,   5,
	   5,  -9,
	-128,   0,
	  -5,   5,
	   6,  -9,
	-128,   0,
	   6,   5,
	  -6,  -9,
	-128,   0,
	  -8,   5,
	  -2,   5,
	-128,   0,
	   2,   5,
	   8,   5,
	-128,   0,
	  -8,  -9,
	  -2,  -9,
	-128,   0,
	   2,  -9,
	   8,  -9,
	-128,   0};
static readonly char ascii_171[ 48] = { 22, '\171', -10,   9,	/* y */
	  -6,   5,
	   0,  -9,
	-128,   0,
	  -5,   5,
	   0,  -7,
	-128,   0,
	   6,   5,
	   0,  -9,
	  -2, -13,
	  -4, -15,
	  -6, -16,
	  -7, -16,
	  -8, -15,
	  -7, -14,
	  -6, -15,
	-128,   0,
	  -8,   5,
	  -2,   5,
	-128,   0,
	   2,   5,
	   8,   5,
	-128,   0};
static readonly char ascii_172[ 36] = { 16, '\172',  -9,   9,	/* z */
	   5,   5,
	  -6,  -9,
	-128,   0,
	   6,   5,
	  -5,  -9,
	-128,   0,
	  -5,   5,
	  -6,   1,
	  -6,   5,
	   6,   5,
	-128,   0,
	  -6,  -9,
	   6,  -9,
	   6,  -5,
	   5,  -9,
	-128,   0};
static readonly char ascii_173[ 60] = { 28, '\173',  -7,   7,	/* { */
	   2,  16,
	  -1,  13,
	  -2,  10,
	  -2,   8,
	  -1,   5,
	   2,   2,
	-128,   0,
	   0,  14,
	  -1,  11,
	  -1,   7,
	   0,   4,
	-128,   0,
	   2,   2,
	  -1,   0,
	   2,  -2,
	-128,   0,
	   2,  -2,
	  -1,  -5,
	  -2,  -8,
	  -2, -10,
	  -1, -13,
	   2, -16,
	-128,   0,
	   0,  -4,
	  -1,  -7,
	  -1, -11,
	   0, -14,
	-128,   0};
static readonly char ascii_174[ 10] = {  3, '\174',  -4,   4,	/* | */
	   0,  16,
	   0, -16,
	-128,   0};
static readonly char ascii_175[ 60] = { 28, '\175',  -7,   7,	/* } */
	  -2,  16,
	   1,  13,
	   2,  10,
	   2,   8,
	   1,   5,
	  -2,   2,
	-128,   0,
	   0,  14,
	   1,  11,
	   1,   7,
	   0,   4,
	-128,   0,
	  -2,   2,
	   1,   0,
	  -2,  -2,
	-128,   0,
	  -2,  -2,
	   1,  -5,
	   2,  -8,
	   2, -10,
	   1, -13,
	  -2, -16,
	-128,   0,
	   0,  -4,
	   1,  -7,
	   1, -11,
	   0, -14,
	-128,   0};
static readonly char ascii_176[ 52] = { 24, '\176', -12,  12,	/* ~ */
	  -9,  -3,
	  -9,  -1,
	  -8,   2,
	  -6,   3,
	  -4,   3,
	  -2,   2,
	   2,  -1,
	   4,  -2,
	   6,  -2,
	   8,  -1,
	   9,   1,
	-128,   0,
	  -9,  -1,
	  -8,   1,
	  -6,   2,
	  -4,   2,
	  -2,   1,
	   2,  -2,
	   4,  -3,
	   6,  -3,
	   8,  -2,
	   9,   1,
	   9,   3,
	-128,   0};
static readonly char ascii_177[  6] = {  1, '\177',  -9,   9,	/*  */
	-128,   0};

/*	Stroke Table	*/

# ifdef VMS
globaldef readonly
# endif
char *Gt_sf_1[128] = {
	size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, ascii_027, 0, 0, 0, 0, 0, 0, 0, 0, 
	ascii_040, ascii_041, ascii_042, ascii_043,
	ascii_044, ascii_045, ascii_046, ascii_047,
	ascii_050, ascii_051, ascii_052, ascii_053,
	ascii_054, ascii_055, ascii_056, ascii_057,
	ascii_060, ascii_061, ascii_062, ascii_063,
	ascii_064, ascii_065, ascii_066, ascii_067,
	ascii_070, ascii_071, ascii_072, ascii_073,
	ascii_074, ascii_075, ascii_076, ascii_077,
	ascii_100, ascii_101, ascii_102, ascii_103,
	ascii_104, ascii_105, ascii_106, ascii_107,
	ascii_110, ascii_111, ascii_112, ascii_113,
	ascii_114, ascii_115, ascii_116, ascii_117,
	ascii_120, ascii_121, ascii_122, ascii_123,
	ascii_124, ascii_125, ascii_126, ascii_127,
	ascii_130, ascii_131, ascii_132, ascii_133,
	ascii_134, ascii_135, ascii_136, ascii_137,
	ascii_140, ascii_141, ascii_142, ascii_143,
	ascii_144, ascii_145, ascii_146, ascii_147,
	ascii_150, ascii_151, ascii_152, ascii_153,
	ascii_154, ascii_155, ascii_156, ascii_157,
	ascii_160, ascii_161, ascii_162, ascii_163,
	ascii_164, ascii_165, ascii_166, ascii_167,
	ascii_170, ascii_171, ascii_172, ascii_173,
	ascii_174, ascii_175, ascii_176, ascii_177
	}; 
