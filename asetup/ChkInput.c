/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"asetup.h"

int ChkInput ()
{
	int		rv = 0;
	
	if ( xsystem.xclosemethod[0] == 'M' )
	{
		xsystem.xretainequity = 0;
		xsystem.xretainincome = 0;
		return ( rv );
	}

	if ( xsystem.xretainequity == 0 )
	{
		SaveError ( "Retained Earnings Equity Account is REQUIRED" );
		rv++;
	}
	else
	{
		if ( LoadOneAccount ( xsystem.xretainequity ) == 0 )
		{
			SaveError ( "Retained Earnings Equity Account NOT FOUND" );
			xsystem.xretainequity = 0;
			rv++;
		}
		else if ( xaccount.xaccttype[0] != 'E' )
		{
			SaveError ( "Retained Earnings Equity Account is not an EQUITY ACCOUNT" );
			xsystem.xretainequity = 0;
			rv++;
		}
	}

	if ( xsystem.xretainincome == 0 )
	{
		SaveError ( "Retained Earnings Income Contra Account is REQUIRED" );
		rv++;
	}
	else
	{
		if ( LoadOneAccount ( xsystem.xretainincome ) == 0 )
		{
			SaveError ( "Retained Earnings Income Contra Account NOT FOUND" );
			xsystem.xretainincome = 0;
			rv++;
		}
		else if ( xaccount.xaccttype[0] != 'I' )
		{
			SaveError ( "Retained Earnings Income Contra Account is not an INCOME ACCOUNT" );
			xsystem.xretainincome = 0;
			rv++;
		}
	}

	if (( xsystem.xretainequity > 0  &&  xsystem.xretainincome == 0 ) ||
		( xsystem.xretainequity == 0  &&  xsystem.xretainincome > 0 ))
	{
		SaveError ( "Both accounts are required" );
		xsystem.xretainincome = 0;
		rv++;
	}

	return ( rv );
}
