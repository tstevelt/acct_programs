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

#include	"impcoa.h"

void PaintScreen ( )
{
	/*--------------------------------------------------------------
		paint javascript
	--------------------------------------------------------------*/
	printf ( "<script language='JavaScript1.2'>\n" );

	printf ( "function DoImport ()\n" );
	printf ( "{\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=600,height=500,resizable=1,menubar=1,scrollbars=1';\n" );
	printf ( "window.open('','impres',options);\n" );
	printf ( "window.document.impform.target = 'impres';\n" );
	printf ( "window.document.impform.submit();\n" );

	printf ( "}\n\n" );

	printf ( "</script>\n" );

	/*--------------------------------------------------------------
		paint html
	--------------------------------------------------------------*/

	printf ( "<table class='AppMedium'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center'>\n" );
	printf ( "<input type='radio' name='autonum' value='A'>\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "automatic numbering based on values entered in setup screen. import format is:<ol><li>account type, which is one of A, L, E, I or X.<li>account name<li>optional, tax form name<li>optional, tax form line</ol>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center'>\n" );
	printf ( "<input type='radio' name='autonum' value='M' checked>\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "account numbers from import file. import format is:<ol><li>account number.<li>account type, which is one of A, L, E, I or X.<li>account name<li>optional, tax form name<li>optional, tax form line</ol>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>filename</td>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input type='file' name='import_filename' size='40'> &nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='4'>\n" );

	printf ( "<input type='button' name='submitImport' value='import' " );
	if ( xmember.xmrole[0] != ROLE_ADMIN )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:DoImport()'>\n" );
	}


	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );


}
