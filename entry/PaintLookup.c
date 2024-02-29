/*----------------------------------------------------------------------------
	Program : PaintLookup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint accounts.
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

#include	"entry.h"

void PaintLookup ()
{
	int		xa;
	char	CurType = '?';

	sprintf  ( StatementTwo, " acctname like '%s%%' ", xaccount.xacctname );
	LoadAccounts ( StatementTwo );

	printf ( "<table class='AppWide'>\n" );

	for ( xa = 0; xa < AcctsCount; xa++ )
	{
		printf ( "<tr>\n" );

		if ( CurType != AcctsArray[xa].xaccttype[0] )
		{
			CurType = AcctsArray[xa].xaccttype[0];

			printf ( "<tr>\n" );
			printf ( "<th colspan='2'>" );
			switch ( CurType )
			{
				case 'A': printf ( "asset" ); break;
				case 'L': printf ( "liability" ); break;
				case 'E': printf ( "equity" ); break;
				case 'I': printf ( "income" ); break;
				case 'X': printf ( "expense" ); break;
			}
			printf ( "</th>" );
			printf ( "</tr>\n" );
		}

		printf ( "<td>" );

		printf ( "<a href=\"#\" onClick='window.opener.document.entry.%s.value=\"%ld\";",
			TargetNumber, AcctsArray[xa].xacctnum );

		printf ( "window.opener.document.entry.%s.value=\"%s\";",
			TargetName, AcctsArray[xa].xacctname );
			
		printf ( "window.close();'>%ld</a>", AcctsArray[xa].xacctnum );

		printf ( "</td>\n" );

		printf ( "<td>" );
		printf ( "%s", AcctsArray[xa].xacctname );
		printf ( "</td>\n" );

		printf ( "</tr>\n" );
	}

	printf ( "</table>\n" );

}
