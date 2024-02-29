/*----------------------------------------------------------------------------
	Program : AddHead.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert invoice header record
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

#include	"ar_entinv.h"

void AddHead ()
{
	sprintf ( StatementOne, 
"insert into arinvh ( customer, invdate, terms, status, ponum ) values ( %ld, '%04d-%02d-%02d', '%s', 'O', '%s' )",
			xarinvh.xcustomer, 
			xarinvh.xinvdate.year4, 
			xarinvh.xinvdate.month, xarinvh.xinvdate.day,
			xarinvh.xterms,
			xarinvh.xponum );

	if ( dbyInsert ( "ar_entinv", &MySql, StatementOne, 0, LOGFILENAME ) !=  1 )
	{
		SaveError ( "Add invoice failed." );
		return;
	}
	
	if (( xarinvh.xid = mysql_insert_id ( &MySql )) == 0 )
	{
		SaveError ( "Add invoice failed to insert invoice header." );
		return;
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "arinvh.id = %ld", xarinvh.xid );
		if ( LoadArinvh ( WhereClause, &xarinvh, 0 ) != 1 )
		{
			SaveError ( "Cannot load new invoice." );
			return;
		}
	}

}
