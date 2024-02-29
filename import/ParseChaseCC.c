/*----------------------------------------------------------------------------
	Program : ParseChaseCC.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Custom function for Chase CC
	Return  : 
----------------------------------------------------------------------------*/

#include	"import.h"

int ParseChaseCC ( char *Buffer, RESULT *Result )
{
	int		rv = 0;

//	printf ( "ChaseCC<br>\n" );

	tokcnt = GetTokensCSV ( Buffer, tokens, MAXTOKS, 0, 0 );

	if ( tokcnt < 6 )
	{
		return ( -1 );
	}

	/*--------------------------------------------------------------
		01234567890
		mm/dd/yyyy
		change from tran date to post date. tms 09/19/2016
	--------------------------------------------------------------*/
	tokens[0][10] = '\0';
	Result->Date.year4  = nsAtoi ( &tokens[0][6] );
	Result->Date.year2  = Result->Date.year4 % 100;
	tokens[0][5] = '\0';
	Result->Date.day   = nsAtoi ( &tokens[0][3] );
	tokens[0][2] = '\0';
	Result->Date.month = nsAtoi ( &tokens[0][0] );

	if ( nsStrcmp ( tokens[4], "Sale" ) == 0 )
	{
		Result->Refnum = "CHARGE";

		Result->Payee = tokens[2];
		Result->Amount = 0.0 - nsAtof ( tokens[5] );
	}
	else if ( nsStrcmp ( tokens[4], "Payment" ) == 0 )
	{
		Result->Refnum = "PAYMENT";
		Result->Payee = "CHASE";
		Result->Amount = 0.0 - nsAtof ( tokens[5] );
	}
	else
	{
		return ( -2 );
	}

	return ( rv );
}
