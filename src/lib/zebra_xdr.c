/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "zebra.h"
/*
 * $Id: zebra_xdr.c,v 2.1 1997-02-10 20:19:58 granger Exp $
 *
 * XDR definitions and interface for basic zebra types and structures
 */

/*
 * Here is the new zebra time format.  This looks suspiciously like the
 * BSD timeval structure....
 */

bool_t
xdr_ZebraTime(xdrs, objp)
	register XDR *xdrs;
	ZebraTime *objp;
{

	register long *buf;

	if (!xdr_long(xdrs, &objp->zt_Sec))
		return (FALSE);
	if (!xdr_long(xdrs, &objp->zt_MicroSec))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_ZebTime(xdrs, objp)
	register XDR *xdrs;
	ZebTime *objp;
{

	register long *buf;

	if (!xdr_ZebraTime(xdrs, objp))
		return (FALSE);
	return (TRUE);
}
/*
 * Locations.
 */

bool_t
xdr_Location(xdrs, objp)
	register XDR *xdrs;
	Location *objp;
{

	register long *buf;

	if (!xdr_float(xdrs, &objp->l_lat))
		return (FALSE);
	if (!xdr_float(xdrs, &objp->l_lon))
		return (FALSE);
	if (!xdr_float(xdrs, &objp->l_alt))
		return (FALSE);
	return (TRUE);
}
/*
 * Scale and bias info for integer-encoded fields.
 */

bool_t
xdr_ScaleInfo(xdrs, objp)
	register XDR *xdrs;
	ScaleInfo *objp;
{

	register long *buf;

	if (!xdr_float(xdrs, &objp->s_Scale))
		return (FALSE);
	if (!xdr_float(xdrs, &objp->s_Offset))
		return (FALSE);
	return (TRUE);
}
/*
 * Regularly-spaced grids in geographical coordinates
 */
	/* X dimension spacing		*/
	/* Y (north/south) spacing	*/
	/* Vertical spacing		*/
	/* Dimensions			*/

bool_t
xdr_RGrid(xdrs, objp)
	register XDR *xdrs;
	RGrid *objp;
{

	register long *buf;

	if (!xdr_float(xdrs, &objp->rg_Xspacing))
		return (FALSE);
	if (!xdr_float(xdrs, &objp->rg_Yspacing))
		return (FALSE);
	if (!xdr_float(xdrs, &objp->rg_Zspacing))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->rg_nX))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->rg_nY))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->rg_nZ))
		return (FALSE);
	return (TRUE);
}
/*
 * Altitude units.  If you add new units here, be sure to add associated
 * units strings and formats to altunits.c.
 */

bool_t
xdr_AltUnitType(xdrs, objp)
	register XDR *xdrs;
	AltUnitType *objp;
{

	register long *buf;

	if (!xdr_enum(xdrs, (enum_t *)objp))
		return (FALSE);
	return (TRUE);
}