/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptgl.h"

void PaintScreen ()
{
	DBY_QUERY		*Query;

	sprintf ( StatementOne, 
		"select count(*) from account where accttype in ('A', 'L', 'E')" );

	if (( Query = dbySelect ( "rptgl", &MySql, StatementOne, LOGFILENAME )) == NULL )
	{
		SaveError ( "No accounts found, Query NULL" );
		return;
	}
	if (( Query->EachRow = mysql_fetch_row ( Query->Result )) == NULL )
	{
		SaveError ( "No accounts found, EachRow NULL" );
		return;
	}

	Count = safe_atoi ( Query->EachRow[0] );
	if ( Count == 0 )
	{
		SaveError ( "No accounts found" );
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
	printf ( "%s", CategoryType == 'I' ? "income statement" : "balance sheet" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	if ( CategoryType == 'I' )
	{
		printf ( "<tr>\n" );
		printf ( "<td>starting date</td>\n" );
		printf ( "<td><input type='search' name='startdate' size='10'></td>\n" );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>%s date</td>\n", CategoryType == 'I' ? "ending" : "as of" );
	printf ( "<td><input type='search' name='enddate' size='10'></td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>include zeros</td><td>\n" );
	printf ( "<select name='IncludeZeros'>\n" );
	printf ( "<option value='N'>No</option>\n" );
	printf ( "<option value='Y'>Yes</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td></tr>\n" );
	
	rptPaintFormat ( "format", ReportFormat );

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
