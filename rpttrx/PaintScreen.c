/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"rpttrx.h"

static int IsSaved ( short AccountNumber )
{
	int		xc;

	for ( xc = 0; xc < Count; xc++ )
	{
		if ( Array[xc] == AccountNumber )
		{
			return ( 1 );
		}
	}

	return ( 0 );
}

void PaintScreen ()
{
	DBY_QUERY		*Query;
	int		xc;
	char	TargetName[48];
	long	SystemTime;

	time ( &SystemTime );
	sprintf ( TargetName, "results%ld", SystemTime );

	printf ( "<script language='JavaScript1.1'>\n" );
	printf ( "<!-- hide code from non-js browsers\n" );

	printf ( "function dofind()\n" );
	printf ( "{\n" );
	printf ( "window.document.rptform.what.value = 'find';\n" );
	printf ( "window.document.rptform.target = '_self';\n" );
	printf ( "window.document.rptform.submit();\n" );
	printf ( "}\n" );

	printf ( "function clearall( Prefix, PreLen )\n" );
	printf ( "{\n" );
	printf ( "var FieldName = 'abc';\n" );
	printf ( "for ( var Index = 0; Index < document.rptform.elements.length; Index++ )\n" );
	printf ( "{\n" );
	printf ( "FieldName = document.rptform.elements[Index].name;\n" );
	printf ( "if ( FieldName.substr(0,PreLen) == Prefix )\n" );
	printf ( "{\n" );
	printf ( "document.rptform.elements[Index].checked = false;\n" );
	printf ( "}\n" );
	printf ( "}\n" );
	printf ( "}\n" );

	printf ( "function setall( Prefix, PreLen )\n" );
	printf ( "{\n" );
	printf ( "var FieldName = 'abc';\n" );
	printf ( "for ( var Index = 0; Index < document.rptform.elements.length; Index++ )\n" );
	printf ( "{\n" );
	printf ( "FieldName = document.rptform.elements[Index].name;\n" );
	printf ( "if ( FieldName.substr(0,PreLen) == Prefix )\n" );
	printf ( "{\n" );
	printf ( "document.rptform.elements[Index].checked = true;\n" );
	printf ( "}\n" );
	printf ( "}\n" );
	printf ( "}\n" );

	printf ( "function runrpt(width,height)\n" );
	printf ( "{\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=' + width + ',height=' + height + ',resizable=1,menubar=1,scrollbars=1';\n" );
	printf ( "window.open ( '', '%s', options );\n", TargetName );
	printf ( "window.document.rptform.what.value = 'go';\n" );
	printf ( "window.document.rptform.target = '%s';\n", TargetName );
	printf ( "window.document.rptform.submit();\n" );
	printf ( "}\n" );

	printf ( "function saverpt(width,height)\n" );
	printf ( "{\n" );
	printf ( "\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=' + width + ',height=' + height + ',resizable=1,menubar=0,scrollbars=0';\n" );
	printf ( "window.open ( '', 'results', options );\n" );
	printf ( "window.document.rptform.what.value = 'save';\n" );
	printf ( "window.document.rptform.target = 'results';\n" );
	printf ( "window.document.rptform.submit();\n" );
	printf ( "}\n" );

	sprintf ( StatementOne, 
		"select %s from rpt where accttype = '%c' order by title",
					FIELDS_RPT, CategoryType );

	xc = 0;
	if (( Query = dbySelect ( "rpttrx", &MySql, StatementOne, LOGFILENAME )) != NULL )
	{
		printf ( "var RptArray = [\n" );

		while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			if ( xc > 0 )
			{
				printf ( ",\n" );
			}

			printf ( " [ '%s', '%s', '%2.2s/%2.2s/%2.2s', '%2.2s/%2.2s/%2.2s' ] ",
					Query->EachRow[RPT_rptname],
					Query->EachRow[RPT_title  ],
					&Query->EachRow[RPT_startdate][5],
					&Query->EachRow[RPT_startdate][8],
					&Query->EachRow[RPT_startdate][2],
					&Query->EachRow[RPT_enddate][5],
					&Query->EachRow[RPT_enddate][8],
					&Query->EachRow[RPT_enddate][2] );

			xc++;
		}

		printf ( " ];\n" );

	}

	printf ( "var RptCount = %d;\n", xc );

	printf ( "function lookup()\n" );
	printf ( "{\n" );
	printf ( "\n" );

	printf ( "if ( RptCount == 0 )\n" );
	printf ( "{\n" );
	printf ( "alert ( 'no saved reports' );\n" );
	printf ( "return;\n" );
	printf ( "}\n" );

	printf ( "var options = 'screenX=10,screenY=10,width=700,height=300,resizable=1,menubar=0,scrollbars=1';\n" );
	printf ( "var w = window.open ( '', 'results', options );\n" );
	printf ( "var d = w.document;\n" );

	printf ( "d.writeln ( '<!DOCTYPE HTML>' );\n" );
	printf ( "d.writeln ( '<html>' );\n" );
	printf ( "d.writeln ( '<head>' );\n" );
	printf ( "d.writeln ( '<title>lookup saved reports</title>' );\n" );
	printf ( "d.writeln ( '</head>' );\n" );
	printf ( "d.writeln ( '<body bgcolor=\"%s\" onLoad=\"window.focus();\">' );\n", COLOR_BACKGROUND );
	printf ( "d.writeln ( '</body>' );\n" );
	printf ( "d.writeln ( '</html>' );\n" );

//	printf ( "d.writeln ( '<table border=\"1\" cellpadding=\"2\" cellspacing=\"0\" width=\"100%%\">' );\n" );
	printf ( "d.writeln ( '<table class=\"AppWide\">' );\n" );
	printf ( "d.writeln ( '<tr>' );\n" );
	printf ( "d.writeln ( '<th>name</th>' );\n" );
	printf ( "d.writeln ( '<th>title</th>' );\n" );
	printf ( "d.writeln ( '<th>start</th>' );\n" );
	printf ( "d.writeln ( '<th>end</th>' );\n" );
	printf ( "d.writeln ( '</tr>' );\n" );
	printf ( "for ( var xc = 0; xc < RptCount; xc++ )\n" );
	printf ( "{\n" );
	printf ( "d.writeln ( '<tr>' );\n" );

	printf ( "d.write ( '<td> <a href=# ' );\n" );
	printf ( "d.write ( 'onClick=javascript:window.opener.document.rptform.rptname.value=\"' );\n" );
	printf ( "d.write ( RptArray[xc][0] );\n" );
	printf ( "d.write ( '\";window.close();>' );\n" );
	printf ( "d.write ( RptArray[xc][0] );\n" );
	printf ( "d.writeln ( '</a></td>' );\n" );

	printf ( "d.writeln ( '<td> ' + RptArray[xc][1] + '</td>' );\n" );
	printf ( "d.writeln ( '<td align=\"center\"> ' + RptArray[xc][2] + '</td>' );\n" );
	printf ( "d.writeln ( '<td align=\"center\"> ' + RptArray[xc][3] + '</td>' );\n" );
	printf ( "d.writeln ( '</tr>' );\n" );
	printf ( "}\n" );
	printf ( "d.writeln ( '</table>' );\n" );
	printf ( "d.close ();\n" );

	printf ( "}\n" );

	printf ( "// end hiding -->\n" );
	printf ( "</script>\n" );


	printf ( "<table class='AppHalf'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );

	switch ( CategoryType )
	{
		case 'A': printf ( "asset report" ); break;
		case 'L': printf ( "liablility report" ); break;
		case 'I': printf ( "income report" ); break;
		case 'X': printf ( "expense report" ); break;
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>report title</td>\n" );
	printf ( "<td><input type='search' name='title' size='50' value='%s'></td>\n", xrpt.xtitle );
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

	printf ( "<tr>\n" );
	printf ( "<td>report name</td>\n" );
	printf ( "<td><input type='search' name='rptname' size='20' value='%s'>", xrpt.xrptname );

	printf ( "&nbsp; <input type='button' value='?' " );
	printf ( "onClick='javascript:lookup();'>" );

	printf ( "&nbsp; <input type='button' value='load' " );
	printf ( "onClick='javascript:dofind();'>\n" );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	rptPaintFormat ( "format", ReportFormat );

	printf ( "</table>\n" );

	printf ( "<table class='AppHalf'>\n" );

	printf ( "<tr>\n" );
	printf ( "<th>x</th>\n" );
	printf ( "<th>category</th>\n" );
	printf ( "</tr>\n" );

	sprintf ( StatementOne, 
		"select %s from account where accttype = '%c' order by acctnum",
					FIELDS_ACCOUNT, CategoryType );

	if (( Query = dbySelect ( "rpttrx", &MySql, StatementOne, LOGFILENAME )) == NULL )
	{
		SaveError ( "No accounts found" );
		return;
	}

	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		printf ( "<tr>\n" );

		xaccount.xacctnum = safe_atol ( Query->EachRow[ACCOUNT_acctnum] );

		printf ( "<td align='center'><input type='checkbox' name='cb_%ld'", xaccount.xacctnum );

		if ( IsSaved ( xaccount.xacctnum ) == 1 )
		{
			printf ( " checked" );
		}

		printf ( "></td>\n" );

		printf ( "<td>%ld - %s</td>\n", xaccount.xacctnum, Query->EachRow[ACCOUNT_acctname] );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );

	printf ( "<input type='button' value='clear' " );
	printf ( "onClick='javascript:clearall(\"cb_\",3);'>\n" );

	printf ( "<input type='button' value='set' " );
	printf ( "onClick='javascript:setall(\"cb_\",3);'>\n" );

	printf ( "<input type='button' value='go' " );
	printf ( "onClick='javascript:runrpt(%d,%d);'>\n" ,
		xsystem.xprintdest[0] == 'L' ? 700 : 400,
		xsystem.xprintdest[0] == 'L' ? 500 : 200 );

	printf ( "<input type='button' value='save' " );
	printf ( "onClick='javascript:saverpt(400,200);'>\n" );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
