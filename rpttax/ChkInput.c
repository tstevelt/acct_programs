/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"rpttax.h"

int ChkInput ()
{
	if ( startdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( enddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

	return ( AcctErrorCount );
}
