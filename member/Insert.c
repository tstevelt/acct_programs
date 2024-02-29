/*----------------------------------------------------------------------------
	Program : Insert.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert member record.
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

#include	"member.h"

void Insert ()
{
	if ( xmember.xid > 0 )
	{
		sprintf ( StatementOne, "insert into member (id, Mname, Memail, Mphone, Mcarrier, Mtwopref, Mrole, Mpassword, Minsdt ) values (%ld, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%04d-%02d-%02d' )",
xmember.xid
, xmember.xmname
, xmember.xmemail
, xmember.xmphone
, xmember.xmcarrier
, xmember.xmtwopref
, xmember.xmrole
, pw_sha_make_pw ( (unsigned char *) xmember.xmpassword )
, xmember.xminsdt.year4, xmember.xminsdt.month, xmember.xminsdt.day );
	}
	else
	{
		sprintf ( StatementOne, "insert into member (Mname, Memail, Mphone, Mcarrier, Mtwopref, Mrole, Mpassword, Minsdt ) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%04d-%02d-%02d' )",
xmember.xmname
, xmember.xmemail
, xmember.xmphone
, xmember.xmcarrier
, xmember.xmtwopref
, xmember.xmrole
, pw_sha_make_pw ( (unsigned char *) xmember.xmpassword )
, xmember.xminsdt.year4, xmember.xminsdt.month, xmember.xminsdt.day );
	}

	if ( dbyInsert ( "member", &MySql, StatementOne, 0, LOGFILENAME ) != 1 )
	{
		SaveError ( "dbyInsert failed" );
		RunMode = MODE_START;
		return;
	}

	if (( xmember.xid = mysql_insert_id ( &MySql )) == 0L )
	{
		SaveError ( "dbyInsert failed to insert new memeber" );
		RunMode = MODE_START;
		return;
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xmember.xid );
		LoadMember ( WhereClause, &xmember, 0 );
	}
}
