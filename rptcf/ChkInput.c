/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptcf.h"

int ChkInput ()
{
	if ( prevdate.month == 0 )
	{
		SaveError ( "missing previous date" );
	}

	if ( currdate.month == 0 )
	{
		SaveError ( "missing current date" );
	}

	return ( AcctErrorCount );
}
