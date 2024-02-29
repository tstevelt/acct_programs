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

#include	"backup.h"

void PaintScreen ()
{
	printf ( "<table class='AppWide'0%%'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "This creates a backup of your database, placed in your file store." );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

// bottom row: save button
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='6'>\n" );
	printf ( "<input name='submitSave' type='submit' value='backup'" );
	if ( xmember.xmrole[0] == ROLE_DEMO )
	{
		printf ( " disabled" );
	}

	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
