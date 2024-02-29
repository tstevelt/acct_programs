/*----------------------------------------------------------------------------
	Program : Delete.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Delete a customer record.
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

int Delete ()
{
	int		Affected;

	if ( xarcust.xid > 0 )
	{
		sprintf ( StatementOne, "delete from arcust where id = %ld", xarcust.xid );
	}
	else
	{
		return ( -1 );
	}

	Affected = dbyDelete ( "ar_arcust", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( Affected < 0 )
	{
		SaveError ( "dbyDelete failed" );
		RunMode = MODE_FOUND;
		return ( -2 );
	}
	else if ( Affected == 0 )
	{
		SaveError ( "Customer not deleted" );
		RunMode = MODE_FOUND;
		return ( -3 );
	}
	else
	{
		RunMode = MODE_START;
	}

	return ( 0 );
}
