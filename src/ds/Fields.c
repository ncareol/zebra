/*
 * Field management for the zeb data store.
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

# include <string.h>

# include <config.h>		/* For CFG_ parameters */
# include <defs.h>
# include <message.h>
# include "ds_fields.h"
MAKE_RCSID ("$Id: Fields.c,v 3.9 1995-02-10 01:09:04 granger Exp $")


/*
 * The structure defining a field.
 */
# define MaxFieldID		CFG_FIELD_MAX_ID
# define MaxFieldName		CFG_FIELD_NAME_LEN
# define MaxFieldLongName 	CFG_FIELD_LONG_LEN
# define MaxFieldUnits		CFG_FIELD_UNITS_LEN

typedef struct _FieldDesc
{
	char	fd_CName[MaxFieldName];		/* Canonical name	*/
	char	fd_LongName[MaxFieldLongName];	/* Full description	*/
	char	fd_Units[MaxFieldUnits];	/* What units it's in ??*/
	/* more to follow */
} FieldDesc;

static FieldDesc FieldTable[MaxFieldID];
static stbl FNameTable;

static int NField = 0;




void
F_Init ()
/*
 * Initialize the fields module.
 */
{
	FNameTable = usy_c_stbl ("FieldNames");
}




FieldId
F_Lookup (name)
const char *name;
/*
 * Turn this name into a field ID.
 */
{
	SValue v;
	int type;

	if (! usy_g_symbol (FNameTable, (char *)name, &type, &v))
		return (F_DeclareField (name, name, "unknown"));
		/* return (BadField); */
	return (v.us_v_int);
}



FieldId
F_Declared (name)
const char *name;
/*
 * Try to find this field.  Return BadField if not found rather than
 * implicitly declaring it.
 */
{
	SValue v;
	int type;

	if (! usy_g_symbol (FNameTable, (char *)name, &type, &v))
		return (BadField);
	else
		return (v.us_v_int);
}



FieldId
F_DeclareField (name, desc, units)
const char *name, *desc, *units;
/*
 * Define this field to the system.
 */
{
	int ind;
	SValue v;
/*
 * Refuse to overwrite existing declarations.  I hope this is the right
 * thing to do, but it will definitely fix a problem seen with netcdf now,
 * where existing long descriptions get wiped out.
 */
	if (usy_defined (FNameTable, (char *)name))
		return (F_Lookup (name));
		/* ind = F_Lookup (name); */
	else
	{
		if ((ind = NField++) == MaxFieldID)
		{
			msg_ELog (EF_EMERGENCY,
				"Cannot declare field '%s'", name);
			msg_ELog (EF_EMERGENCY, 
				"MaxFieldID too small in Fields.c");
			NField--;
			return (BadField);
		}
	}
	strncpy (FieldTable[ind].fd_CName, name, MaxFieldName);
	FieldTable[ind].fd_CName[MaxFieldName - 1] = '\0';
	strncpy (FieldTable[ind].fd_LongName, desc, MaxFieldLongName);
	FieldTable[ind].fd_LongName[MaxFieldLongName - 1] = '\0';
	strncpy (FieldTable[ind].fd_Units, units, MaxFieldUnits);
	FieldTable[ind].fd_Units[MaxFieldUnits - 1] = '\0';

	v.us_v_int = ind;
	usy_s_symbol (FNameTable, (char *)name, SYMT_INT, &v);
	return (ind);
}





FieldId
F_Alias (name, alias)
const char *name, *alias;
/*
 * Cause "alias" to be equivalent to the existing field "name".
 * Fails and returns BadField if "name" does not exist.
 */
{
	int index;
	SValue v;

	if ((index = F_Declared (name)) == BadField)
		return (BadField);
	v.us_v_int = index;
	usy_s_symbol (FNameTable, (char *)alias, SYMT_INT, &v);
	return (index);
}





char *
F_GetName (id)
FieldId id;
/*
 * Turn this ID back into a name.
 */
{
	if (id < 0 || id >= NField)
		return (0);
	return (FieldTable[id].fd_CName);
}



char *
F_GetDesc (id)
FieldId id;
/*
 * Turn this ID back into a description.
 */
{
	if (id < 0 || id >= NField)
		return (0);
	return (FieldTable[id].fd_LongName);
}


char *
F_GetUnits (id)
FieldId id;
/*
 * Turn this ID into a unit amount.
 */
{
	if (id < 0 || id >= NField)
		return (0);
	return (FieldTable[id].fd_Units);
}
