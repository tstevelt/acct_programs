/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"rpttrx.h"

int ChkInput ()
{
//	snprintf ( title, sizeof(title), "%s", webValues[xa] );
//	snprintf ( rptname, sizeof(rptname), "%s", webValues[xa] );

	switch ( CategoryType )
	{
		case 'I':
		case 'X':
		case 'A':
		case 'L':
			break;
		default :
			SaveError ( "invalid type, use A L I or X" );
			break;
	}

	if ( RunMode == MODE_FIND )
	{
		if ( nsStrlen ( xrpt.xrptname ) == 0 )
		{
			SaveError ( "please enter report name" );
		}

		return ( AcctErrorCount );
	}

	if ( xrpt.xstartdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( xrpt.xenddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

	if ( Count == 0 )
	{
		SaveError ( "no categories selected" );
	}

//	sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
//	SaveError ( StatementOne );

	return ( AcctErrorCount );
}
