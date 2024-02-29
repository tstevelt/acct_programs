/*----------------------------------------------------------------------------
	Program : SaveAccount.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update account record
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

int SaveAccount ( void )
{
	int		rv = 0;
	XACCOUNT	NewAccount;

	if ( xaccount.xacctname[0] == '\0' )
	{
		sprintf ( StatementOne, "account names are required.\n" );
		SaveError ( StatementOne );
		return ( rv );
	}

	memcpy ( &NewAccount, &xaccount, sizeof(XACCOUNT) );

	if ( LoadOneAccount ( xaccount.xacctnum ) == 0 )
	{
		sprintf ( StatementOne, "can no load account prior to saving.\n" );
		SaveError ( StatementOne );
		return ( rv );
	}

	NewAccount.xcurrbal = xaccount.xcurrbal - xaccount.xopenbal + NewAccount.xopenbal;

	sprintf ( StatementOne, 
"update account set acctname = '%s', extranum = '%s', extraline = '%s', openbal = %ld, currbal = %ld, \
recondate = '%04d-%02d-%02d', reconbal = %ld, daterange = %d where acctnum = %ld",
			NewAccount.xacctname,
			NewAccount.xextranum,
			NewAccount.xextraline,
			NewAccount.xopenbal,
			NewAccount.xcurrbal,
			NewAccount.xrecondate.year4, NewAccount.xrecondate.month, NewAccount.xrecondate.day, 
			NewAccount.xreconbal,
			NewAccount.xdaterange,
			NewAccount.xacctnum );
	
	rv = dbyUpdate  ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	
	return ( rv );
}
