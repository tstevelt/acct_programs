/*----------------------------------------------------------------------------
	Program : AddImport.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert inport record
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

int AddImport ( short AccountNumber )
{
	int		rv;
	
#ifdef VERSION_ONE
	sprintf ( StatementOne,
		"insert into import (acctnum, autoparse) values ( %d, 'Y' )", AccountNumber );
#else
	sprintf ( StatementOne,
		"insert into import (acctnum) values ( %d )", AccountNumber );
#endif

// printf ( "%s<br>\n", StatementOne );

	rv = dbyInsert ( "accounts", &MySql, StatementOne, 0, LOGFILENAME );

	if ( rv != 1 )
	{
		rv = 0;
	}

	return ( rv );
}

