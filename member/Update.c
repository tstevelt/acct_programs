/*----------------------------------------------------------------------------
	Program : Update.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update member record
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

void Update ()
{
	time_t	TimeStamp;
	int		Affected;

	TimeStamp = time ( NULL );

	if ( xmember.xid > 0 )
	{
		sprintf ( StatementOne, "update member set Mname = '%s', Memail = '%s', Mphone = '%s', Mcarrier = '%s', Mtwopref = '%s', Mrole = '%s' where id = %ld",
xmember.xmname
, xmember.xmemail
, xmember.xmphone
, xmember.xmcarrier
, xmember.xmtwopref
, xmember.xmrole
, xmember.xid );
	}

	Affected = dbyUpdate ( "member", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( Affected < 0 )
	{
		SaveError ( "dbyUpdate failed" );
		RunMode = MODE_FOUND;
		return;
	}
	else if ( Affected == 0 )
	{
		SaveError ( "No changes" );
		RunMode = MODE_FOUND;
		return;
	}
	else
	{
		sprintf ( StatementOne, "update member set updated = %ld where id = %ld", TimeStamp, xmember.xid );
		dbyUpdate ( "member", &MySql, StatementOne, 0, LOGFILENAME );
	}

	snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xmember.xid );
	LoadMember ( WhereClause, &xmember, 0 );
}
