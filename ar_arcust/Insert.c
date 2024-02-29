/*----------------------------------------------------------------------------
	Program : Insert.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert a customer record.
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

#include	"ar_arcust.h"

void Insert ()
{
	if ( xarcust.xid > 0 )
	{
		sprintf ( StatementOne, 
	"insert into arcust (id, name, addr1, addr2, city, state, zipcode, phone, contact ) values ( %ld, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )",
			xarcust.xid,
			xarcust.xname,
			xarcust.xaddr1,
			xarcust.xaddr2,
			xarcust.xcity,
			xarcust.xstate,
			xarcust.xzipcode,
			xarcust.xphone,
			xarcust.xcontact );

	}
	else
	{
		sprintf ( StatementOne, 
	"insert into arcust ( name, addr1, addr2, city, state, zipcode, phone, contact ) values ( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )",
			xarcust.xname,
			xarcust.xaddr1,
			xarcust.xaddr2,
			xarcust.xcity,
			xarcust.xstate,
			xarcust.xzipcode,
			xarcust.xphone,
			xarcust.xcontact );
	}

	if ( dbyInsert ( "ar_arcust", &MySql, StatementOne, 0, LOGFILENAME ) != 1 )
	{
		SaveError ( "dbyInsert failed" );
		RunMode = MODE_START;
		return;
	}

	if (( xarcust.xid = mysql_insert_id ( &MySql )) == 0 )
	{
		SaveError ( "dbyInsert failed to insert new customer" );
		RunMode = MODE_START;
		return;
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xarcust.xid );
		LoadArcust ( WhereClause, &xarcust, 0 );
	}
}
