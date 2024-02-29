/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
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

#include	"ar_statement.h"

int ChkInput ()
{
	int		Count;

	if ( CustomerNumber == 0 )
	{
		SaveError ( "Missing Customer Number" );
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", CustomerNumber );
		if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
		{
			SaveError ( "Invalid Customer Number" );
		}
	}

	snprintf ( WhereClause, sizeof(WhereClause), "customer = %ld and status = 'O'", xarcust.xid );
//	printf ( "%s\n", WhereClause );
	Count = dbySelectCount ( &MySql, "arinvh", WhereClause, LOGFILENAME );
	if ( Count == 0 )
	{
		SaveError ( "Customer does not have any open invoices" );
	}

	return ( AcctErrorCount );
}
