/*----------------------------------------------------------------------------
	Program : Load.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load a customer record.
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

static	int		DebugLoad = 0;

int Load ()
{
	int		rv = 0;

	RunMode = MODE_FOUND;

	snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarcust.xid );
	if (( rv = LoadArcust ( WhereClause, &xarcust, 0 )) != 1 )
	{
		if ( DebugLoad )
		{
			sprintf ( msgbuf, "Unknown customer ID %ld, rv = %d", xarcust.xid, rv );
			SaveError ( msgbuf );
		}
		else
		{
			SaveError ( "Unknown customer ID" );
		}
		RunMode = MODE_START;
	}

	return ( rv );
}
