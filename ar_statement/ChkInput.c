/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_statement.h"

int ChkInput ()
{
	int		Count;

	if ( CustomerNumber == 0 )
	{
		SaveError ( "Missing Customer Number" );
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", CustomerNumber );
		if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
		{
			SaveError ( "Invalid Customer Number" );
		}
	}

	snprintf ( WhereClause, sizeof(WhereClause), "customer = %ld and status = 'O'", xarcust.xid );
//	printf ( "%s\n", WhereClause );
	Count = dbySelectCount ( &MySql, "arinvh", WhereClause, LOGFILENAME );
	if ( Count == 0 )
	{
		SaveError ( "Customer does not have any open invoices" );
	}

	return ( AcctErrorCount );
}
