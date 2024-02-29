/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptall.h"

int ChkInput ()
{
//	snprintf ( title, sizeof(title), "%s", webValues[xa] );
//	snprintf ( rptname, sizeof(rptname), "%s", webValues[xa] );

	if ( xrpt.xstartdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( xrpt.xenddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

//	sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
//	SaveError ( StatementOne );

	return ( AcctErrorCount );
}
