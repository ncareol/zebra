/*
 * Code for converting old-style data objects into new data chunks.
 */
/*		Copyright (C) 1987,88,89,90,91,92 by UCAR
 *	University Corporation for Atmospheric Research
 *		   All rights reserved
 *
 * No part of this work covered by the copyrights herein may be reproduced
 * or used in any form or by any means -- graphic, electronic, or mechanical,
 * including photocopying, recording, taping, or information storage and
 * retrieval systems -- without permission of the copyright owner.
 * 
 * This software and any accompanying written materials are provided "as is"
 * without warranty of any kind.  UCAR expressly disclaims all warranties of
 * any kind, either express or implied, including but not limited to the
 * implied warranties of merchantibility and fitness for a particular purpose.
 * UCAR does not indemnify any infringement of copyright, patent, or trademark
 * through use or modification of this software.  UCAR does not provide 
 * maintenance or updates for its software.
 */

# include <defs.h>
# include <message.h>
# include "DataStore.h"
# include "ds_fields.h"
# include "DataChunk.h"
MAKE_RCSID ("$Id: ConvertDObj.c,v 1.1 1991-12-04 23:45:14 corbet Exp $")



# ifdef __STDC__
	static DataChunk *MakeIRGrid (DataObject *);
	static DataChunk *MakeBoundary (DataObject *);
	static DataChunk *MakeScalar (DataObject *);
	static DataChunk *Make2dGrid (DataObject *);
# else
# endif



DataChunk *
ConvertDObj (dobj)
DataObject *dobj;
/*
 * Convert this data object into a data chunk.
 */
{
/*
 * Just pass the work out to the organization-specific converter.
 */
	switch (dobj->do_org)
	{
	   case OrgIRGrid:
	   	return (MakeIRGrid (dobj));

	   case OrgOutline:
	   	return (MakeBoundary (dobj));

	   case Org2dGrid:
	   	return (Make2dGrid (dobj));

	   case OrgScalar:
	   	return (MakeScalar (dobj));

	   default:
	   	msg_ELog (EF_PROBLEM, "Can't convert org %d yet",dobj->do_org);
		return (NULL);
	}
}




static DataChunk *
MakeIRGrid (dobj)
DataObject *dobj;
/*
 * Create an irregular grid chunk.
 */
{
	DataChunk *dc = dc_CreateDC (DCC_IRGrid);
	FieldId fids[30];
	IRGrid *irg = &dobj->do_desc.d_irgrid;
	int field, sample;
	float *dp;
/*
 * Initial setup.
 */
	dc->dc_Platform = dobj->do_id;
/*
 * Convert the field names in the DO to ID's, then set up our DC.
 */
	for (field = 0; field < dobj->do_nfield; field++)
		fids[field] = F_Lookup (dobj->do_fields[field]);
	dc_IRSetup (dc, irg->ir_npoint, irg->ir_subplats, irg->ir_loc,
			dobj->do_nfield, fids);
/*
 * Go through and add each field to the new data chunk.
 */
	for (field = 0; field < dobj->do_nfield; field++)
	{
		dp = dobj->do_data[field];
		for (sample = 0; sample < dobj->do_npoint; sample++)
		{
			dc_IRAddGrid (dc, dobj->do_times + sample, sample,
					fids[field], dp);
			dp += dobj->do_npoint;
		}
	}
/*
 * That's it!
 */
	return (dc);
}



static DataChunk *
MakeBoundary (dobj)
DataObject *dobj;
/*
 * Make a boundary format data chunk.
 */
{
	DataChunk *dc = dc_CreateDC (DCC_Boundary);

/*
 * Here, we just add the single boundary that we expect to be there.  Old
 * data objects didn't work right with more than that anyway....
 */
	dc->dc_Platform = dobj->do_id;
	dc_BndAdd (dc, dobj->do_times, dobj->do_id, dobj->do_aloc, 
			dobj->do_desc.d_bnd->bd_npoint);
	return (dc);
}




static DataChunk *
Make2dGrid (dobj)
DataObject *dobj;
/*
 * Deal with this two-dim grid.
 */
{
	DataChunk *dc = dc_CreateDC (DCC_RGrid);
	FieldId fids[30];
	RGrid *rg = &dobj->do_desc.d_rgrid;
	int field, sample;

	dc->dc_Platform = dobj->do_id;
/*
 * Convert the field names in the DO to ID's, then set up our DC.
 */
	for (field = 0; field < dobj->do_nfield; field++)
		fids[field] = F_Lookup (dobj->do_fields[field]);
	dc_RGSetup (dc, dobj->do_nfield, fids);
/*
 * Go through and add each grid.
 */
	rg->rg_nZ = 1;	/* Just to be sure.	*/
	for (field = 0; field < dobj->do_nfield; field++)
		dc_RGAddGrid (dc, 0, fids[field], &dobj->do_loc, rg,
			dobj->do_times, dobj->do_data[field], 0);
	return (dc);
}






static DataChunk *
MakeScalar (dobj)
DataObject *dobj;
/*
 * Convert this scalar data object.
 */
{
	int field, sample;
	FieldId fids[30];
	DataChunk *dc = dc_CreateDC (DCC_Scalar);

	dc->dc_Platform = dobj->do_id;
/*
 * Convert the field names in the DO to ID's, then set up our DC.
 */
	for (field = 0; field < dobj->do_nfield; field++)
		fids[field] = F_Lookup (dobj->do_fields[field]);
	dc_SetScalarFields (dc, dobj->do_nfield, fids);
/*
 * Now we just go through and add the data, one field at a time.
 */
	for (field = 0; field < dobj->do_nfield; field++)
		dc_AddMultScalar (dc, dobj->do_times, 0, dobj->do_npoint,
				fids[field], dobj->do_data[field]);
/*
 * Set the location for this chunk.  If it is a mobile platform, we need
 * to add each individually.
 */
	if (ds_IsMobile (dobj->do_id))
		dc_SetStaticLoc (dc, &dobj->do_loc);
	else
		for (sample = 0; sample < dobj->do_npoint; sample++)
			dc_SetLoc (dc, sample, dobj->do_aloc + sample);
	return (dc);
}
