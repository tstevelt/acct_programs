/*----------------------------------------------------------------------------
	Program : ProcessForgot.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update member record with new password
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

#include	"chpw.h"

static int DebugProcess = 0;

int ProcessForgot ()
{
	char	WhereClause[512];
	long	Affected;

	if ( DebugProcess )
	{
		printf ( "db=[%s]  user=[%s]<br>\n", Database, MemberName );
	}

	StartDb ( Database );

	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", MemberName );
	if ( LoadMember ( WhereClause, &xmember, 0 ) != 1 )
	{
		SaveError ( "Cannot find member record" );
		return ( -1 );
	}

	if ( DebugProcess )
	{
		printf ( "last login %d/%d/%d<br>\n", 
			xmember.xmlogdt.month, xmember.xmlogdt.day, xmember.xmlogdt.year2 );
		printf ( "old password %s<br>\n", xmember.xmpassword );
	}


	sprintf ( StatementOne, "update member set Mpassword = '%s' where Mname = '%s'",
				pw_sha_make_pw ( (unsigned char *)NewPassword), MemberName );

	if (( Affected = dbyUpdate ( "chpw", &MySql, StatementOne, 0, LOGFILENAME )) != 1 )
	{
		sprintf ( msgbuf, "%ld records updated", Affected );
		SaveError ( msgbuf );
		return ( -1 );
	}

	return ( 0 );
}
