/*----------------------------------------------------------------------------
	Program : FindTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Find next transaction to display
	Return  : 
----------------------------------------------------------------------------*/

#include	"entry.h"

int	FirstTrx = 0;
int	LastTrx = 0;

long FindTrx ( long OnScreen, int direction )
{
	long	NewTrx = 0L;
	long	EndPoint;
	DBY_QUERY		*QueryOne;

	if ( direction == -1 )
	{
		sprintf ( StatementOne,
			"select max(trxnum), min(trxnum) from trxh where trxnum < %ld",
				OnScreen );
	}
	else
	{
		sprintf ( StatementOne,
			"select min(trxnum), max(trxnum) from trxh where trxnum > %ld",
				OnScreen );
	}
	
	QueryOne = dbySelect ( "accounts", &MySql, StatementOne, LOGFILENAME );

	if (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		NewTrx   = safe_atoi ( QueryOne->EachRow [ 0 ] );
		EndPoint = safe_atoi ( QueryOne->EachRow [ 1 ] );

		if ( NewTrx == EndPoint )
		{
			if ( direction == 1 )
			{
				LastTrx = 1;
			}
			else
			{
				FirstTrx = 1;
			}
		}
	}

	if ( NewTrx == 0L )
	{
		NewTrx = OnScreen;
	}

	return ( NewTrx );
}
