/*----------------------------------------------------------------------------
	Program : FindTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Find next transaction to display
	Return  : 
----------------------------------------------------------------------------*/
//     Accounting Programs
// 
//     Copyright (C)  2000-2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
