/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptgl.h"

int ChkInput ()
{
//	snprintf ( title, sizeof(title), "%s", webValues[xa] );
//	snprintf ( rptname, sizeof(rptname), "%s", webValues[xa] );

	switch ( CategoryType )
	{
		case 'B':
		case 'I':
			break;
		default :
			SaveError ( "invalid type, use 'B' or 'I'" );
			break;
	}

	if ( CategoryType == 'I' && startdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( enddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

//	sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
//	SaveError ( StatementOne );

	return ( AcctErrorCount );
}
