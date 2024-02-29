/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
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

#include	"close.h"

void PaintScreen ()
{
	printf ( "<table class='AppHalf'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>current close date</td>\n" );
	printf ( "<td>\n" );
	if ( xsystem.xclosedate.month > 0 )
	{
		printf ( "%02d/%02d/%02d\n",
				xsystem.xclosedate.month, xsystem.xclosedate.day, xsystem.xclosedate.year2 );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>close date</td>\n" );
	printf ( "<td><input type='search' name='closedate' size='10'></td>\n" );
	printf ( "</tr>\n" );


	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );

	if ( xmember.xmrole[0] == ROLE_ADMIN )
	{
		printf ( "<input type='button' value='close' " );
		printf ( "onClick='javascript:what.value=\"close\";submit();'>\n" );
	}
	else
	{
		printf ( "close can only be run by an administrator.\n" );
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
