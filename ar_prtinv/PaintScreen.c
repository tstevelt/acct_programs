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

#include	"ar_prtinv.h"

void PaintScreen ()
{
	int			Count;

	Count = dbySelectCount ( &MySql, "arinvh", NULL, LOGFILENAME );
	if ( Count == 0 )
	{
		SaveError ( "No invoices found" );
		return;
	}

	printf ( "<script language='JavaScript1.1'>\n" );
	printf ( "<!-- hide code from non-js browsers\n" );

	printf ( "function runrpt(width,height)\n" );
	printf ( "{\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=' + width + ',height=' + height + ',resizable=1,menubar=1,scrollbars=1';\n" );
	printf ( "window.open ( '', 'results', options );\n" );
	printf ( "window.document.rptform.what.value = 'go';\n" );
	printf ( "window.document.rptform.target = 'results';\n" );
	printf ( "window.document.rptform.submit();\n" );
	printf ( "}\n" );

	printf ( "// end hiding -->\n" );
	printf ( "</script>\n" );


	printf ( "<table class='AppHalf'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );
	printf ( "print invoice" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Invoice Number</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='InvoiceNumber' size='8'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Include Payment Instructions?</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='IncludeInstructions'>\n" );
	printf ( "<option value='Y'>Yes</option>\n" );
	printf ( "<option value='N'>No</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Send to</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='ReportFormat'>\n" );
	printf ( "<option value='%c'>Screen</option>\n", RPT_FORMAT_HTML );
	printf ( "<option value='%c'>Email</option>\n", RPT_FORMAT_PDF_EMAIL );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );


	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );

	printf ( "<input type='button' value='go' " );
	printf ( "onClick='javascript:what.value=\"go\";runrpt(%d,%d);'>\n" ,
		xsystem.xprintdest[0] == 'L' ? 700 : 400,
		xsystem.xprintdest[0] == 'L' ? 500 : 200 );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
