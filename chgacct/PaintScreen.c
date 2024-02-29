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

#include	"chgacct.h"

void PaintScreen ()
{
	char	*Action;

	printf ( "<table class='AppMedium'>\n" );

// title
	printf ( "<tr>\n" );
	printf ( "<td colspan='3' align='center'>\n" );
	printf ( "Change existing account number to new number" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

// current account number
	printf ( "<tr>\n" );
	printf ( "<td>current account number</td>\n" );
	printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
		printf ( "<input type='search' name='CurrentAcctnum' " );
		if ( CurrentAcctnum > 0 )
		{
			printf ( " value='%ld'", CurrentAcctnum );
		}
	}
	else
	{
		printf ( "%ld", CurrentAcctnum );
		printf ( "<input type='hidden' name='CurrentAcctnum' value='%ld'", CurrentAcctnum );
	}
	printf ( "></td>\n" );
	printf ( "<td>\n" );
	if ( strlen ( CurrentName ) > 0 )
	{
		printf ( "%s", CurrentName );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

// new account number
	printf ( "<tr>\n" );
	printf ( "<td>new account number</td>\n" );
	printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
		printf ( "<input type='search' name='NewAcctnum' " );
		if ( NewAcctnum > 0 )
		{
			printf ( " value='%ld'", NewAcctnum );
		}
	}
	else
	{
		printf ( "%ld", NewAcctnum );
		printf ( "<input type='hidden' name='NewAcctnum' value='%ld'", NewAcctnum );
	}
	printf ( "></td>\n" );
	printf ( "<td>\n" );
	if ( strlen ( NewName ) > 0 )
	{
		printf ( "%s", NewName );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );


// bottom row: check or save button
	switch ( RunMode )
	{
		case MODE_CHECK:
			Action = "save";
			break;
		case MODE_SAVE:
			Action = "start";
			break;
		case MODE_START:
			Action = "check";
			break;
	}

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='3'>\n" );
	printf ( "<input type='button' value='%s' ", Action );
	if ( xmember.xmrole[0] != ROLE_ADMIN )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:what.value=\"%s\";submit();'> &nbsp;\n", Action );
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
