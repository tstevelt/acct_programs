/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_prtinv.h"

int ChkInput ()
{
	char	WhereClause[256];

	if ( InvoiceNumber == 0 )
	{
		SaveError ( "Missing Invoice Number" );
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", InvoiceNumber );
		if ( LoadArinvh ( WhereClause, &xarinvh, 0 ) != 1 )
		{
			SaveError ( "Invalid Invoice Number" );
		}
	}

	return ( AcctErrorCount );
}
