/*----------------------------------------------------------------------------
	Program : PaintUsers.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint users list.
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

#include	"asetup.h"

int PaintUsers ()
{
	DBY_QUERY		*Query;
	int				Rows = 0;

	sprintf ( StatementOne, "select %s from member order by Mname", FIELDS_MEMBER );

	Query = dbySelect ( "setup", &MySql, StatementOne, LOGFILENAME );

	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		Rows++;

		if ( Rows == 1 )
		{
			printf ( "<tr>\n" );
			printf ( "<th>id</th>\n" );
			printf ( "<th>name</th>\n" );
			printf ( "<th>last login</th>\n" );
			printf ( "</tr>\n" );
		}

		printf ( "<tr>\n" );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_id] );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_Mname] );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_Mlogdt] );
		printf ( "</tr>\n" );


	}

	return ( Rows );
}
