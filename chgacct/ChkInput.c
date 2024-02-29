/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"chgacct.h"

int ChkInput ()
{
	int		rv = 0;

	if ( CurrentAcctnum < 1 || NewAcctnum < 1 )
	{
		SaveError ( "Account Numbers are required" );
		rv++;
	}

	if ( CurrentAcctnum > 0 && LoadOneAccount ( CurrentAcctnum ) != 1 )
	{
		SaveError ( "Invalid Current Account Number" );
		rv++;
	}
	else
	{
		sprintf ( CurrentName, "%s", xaccount.xacctname );
	}

	if ( NewAcctnum > 0 && LoadOneAccount ( NewAcctnum ) == 1 )
	{
		SaveError ( "New Account Number already exists" );
		sprintf ( NewName, "%s", xaccount.xacctname );
		rv++;
	}

	return ( rv );
}
