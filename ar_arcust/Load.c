/*----------------------------------------------------------------------------
	Program : Load.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load a customer record.
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

static	int		DebugLoad = 0;

int Load ()
{
	int		rv = 0;

	RunMode = MODE_FOUND;

	snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarcust.xid );
	if (( rv = LoadArcust ( WhereClause, &xarcust, 0 )) != 1 )
	{
		if ( DebugLoad )
		{
			sprintf ( msgbuf, "Unknown customer ID %ld, rv = %d", xarcust.xid, rv );
			SaveError ( msgbuf );
		}
		else
		{
			SaveError ( "Unknown customer ID" );
		}
		RunMode = MODE_START;
	}

	return ( rv );
}
