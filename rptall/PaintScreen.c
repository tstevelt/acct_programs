/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptall.h"

void PaintScreen ()
{
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
	printf ( "transaction list" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>starting date</td>\n" );

	printf ( "<td><input type='search' name='startdate' size='10' " );
	if ( xrpt.xstartdate.month > 0 )
	{
		printf ( "value='%02d/%02d/%02d'",
			xrpt.xstartdate.month, xrpt.xstartdate.day, xrpt.xstartdate.year2 );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>ending date</td>\n" );
	printf ( "<td><input type='search' name='enddate' size='10' " );
	if ( xrpt.xenddate.month > 0 )
	{
		printf ( "value='%02d/%02d/%02d'",
			xrpt.xenddate.month, xrpt.xenddate.day, xrpt.xenddate.year2 );
	}
	printf ( "></td>\n" );
	printf ( "</tr>\n" );

	rptPaintFormat ( "format", ReportFormat );

	printf ( "<tr>\n" );
	printf ( "<td colspan='2' align='center'>\n" );

	printf ( "<input type='button' value='go' " );
	printf ( "onClick='javascript:runrpt(%d,%d);'>\n" ,
		xsystem.xprintdest[0] == 'L' ? 700 : 400,
		xsystem.xprintdest[0] == 'L' ? 500 : 200 );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
