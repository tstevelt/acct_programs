/*----------------------------------------------------------------------------
	Program : Delete.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Delete member record.
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

int Delete ()
{
	int		Affected;

	if ( xmember.xid > 0 )
	{
		sprintf ( StatementOne, "delete from member where id = %ld", xmember.xid );
	}
	else
	{
		return ( -1 );
	}

	Affected = dbyDelete ( "member", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( Affected < 0 )
	{
		SaveError ( "dbyDelete failed" );
		RunMode = MODE_FOUND;
		return ( -2 );
	}
	else if ( Affected == 0 )
	{
		SaveError ( "Customer not deleted" );
		RunMode = MODE_FOUND;
		return ( -3 );
	}
	else
	{
		RunMode = MODE_START;
	}

	return ( 0 );
}
