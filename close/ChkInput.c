/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"close.h"

int ChkInput ()
{
	if ( closedate.month == 0 )
	{
		SaveError ( "missing close date" );
	}

	if ( xsystem.xclosedate.month > 0 )
	{
		if ( CompareDateval ( &closedate, &xsystem.xclosedate ) <= 0 )
		{
			SaveError ( "close date less than or equal to current close date" );
		}
	}

	return ( AcctErrorCount );
}
