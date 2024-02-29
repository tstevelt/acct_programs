/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create Tax Forms report
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
/*------------------------------------------------------------------------------
						tax forms
						 tom stevelt
						 p. o. box 37622
						 cincinnati, oh 45222

----------------------------------------------------------------------------------
tax form             form line            account                           amount 
x20xxxxxxxxxxxxxxxxx x20xxxxxxxxxxxxxxxxx x30xxxxxxxxxxxxxxxxxxxxxxxxxxx 999999.99
123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-12
------------------------------------------------------------------------------*/

#include	"rpttax.h"

static	int		DebugReport = 0;

int getdata ()
{
	int		lineno = 0;
	char		Fragment[40];
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	long		account_total;
	long		grand_total;

	sprintf ( fnData, "%s/rpttax_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	sprintf ( StatementOne,
		"select %s  from account where accttype in ('I','X') and extraline != '' ",
				FIELDS_ACCOUNT );

	strcat ( StatementOne, "order by extranum, extraline, acctname" );

	if ( DebugReport )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	grand_total= 0L;

	QueryOne = dbySelect ( "rpttax", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		sprintf ( StatementTwo,
			"select sum(amount) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate <= '%04d-%02d-%02d'",
				QueryOne->EachRow[ACCOUNT_acctnum],
				enddate.year4, enddate.month, enddate.day );

		sprintf ( Fragment,
			" and trxdate >= '%04d-%02d-%02d'",
				startdate.year4, startdate.month, startdate.day );

		strcat ( StatementTwo, Fragment );

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rpttax", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			account_total= 0L;
		}
		else
		{
			account_total= safe_atol ( QueryTwo->EachRow[0] );
		}


		if ( account_total != 0 )
		{
			fprintf ( fpData, "%-20.20s|%-20.20s|%-30.30s|%9.2f\n",
					QueryOne->EachRow[ACCOUNT_extranum],
					QueryOne->EachRow[ACCOUNT_extraline], 
					QueryOne->EachRow[ACCOUNT_acctname], 
					(double) account_total / 100.0 );

			lineno++;

			grand_total += account_total;

			if ( DebugReport )
			{
				printf ( "account_total %ld grand %ld<br>\n", account_total, grand_total );
			}
		}

	}

	nsFclose ( fpData );

	return ( lineno );
}

static	COLUMN_HEADINGS	ColumnArray [] = 
{
	{ "tax form",	"",			INIT_STRING_LEFT },
	{ "form line",	"",			INIT_STRING_LEFT },
	{ "account",	"",			INIT_STRING_LEFT },
	{ "amount",		"",			INIT_DOUBLE_RIGHT },
};

static  int		ColumnCount = sizeof(ColumnArray) / sizeof(COLUMN_HEADINGS);

void dorpt ()
{
	char	xbuffer[1024];
#define		MAXTOKS		10
	char	*tokens[MAXTOKS];
	int		tokcnt;
	char	Title[80];
	char	SubTitle[80];

	sprintf ( Title, "tax forms - %s", xsystem.xname );

	sprintf ( SubTitle, "%02d/%02d/%04d - %02d/%02d/%04d",
			startdate.month, startdate.day, startdate.year4,
			enddate.month, enddate.day, enddate.year4 );

	ReportOptions.HeaderRows = 1;
	ReportOptions.Title = Title;
	ReportOptions.Subtitle = SubTitle;
	ReportOptions.Format = ReportFormat;
	switch ( ReportOptions.Format )
	{
		case RPT_FORMAT_CSV:
		case RPT_FORMAT_EXCEL:
		case RPT_FORMAT_HTML:
			ReportOptions.LinesPerPage = 0;
			ReportOptions.WritePageNumbers = 0;
			break;
		case RPT_FORMAT_TEXT:
		case RPT_FORMAT_PDF_VIEW:
		case RPT_FORMAT_PDF_EMAIL:
			ReportOptions.LinesPerPage = 60;
			ReportOptions.WritePageNumbers = 1;
			break;
	}
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rpttax", getpid() );
	strcat ( ReportOptions.OutputFilename, rptextension ( ReportOptions.Format ) );
	ReportOptions.IsStdout = 0;

	fpData = rptinit ( fnData, &ReportOptions, ColumnArray, ColumnCount );

	if ( DebugReport )
	{
		printf ( "fnData                  %s<br>\n", fnData );
		printf ( "ReportOptions.OutputFilename %s<br>\n", ReportOptions.OutputFilename );
		printf ( "File Pointer %s<br>\n", fpData == NULL ? "null" : "okay" );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fpData ) != (char *)0 )
	{
		tokcnt = GetTokensA ( xbuffer, "|\n\r", tokens, MAXTOKS );

		if ( DebugReport )
		{
			printf ( "tokcnt %d<br>\n", tokcnt );
		}

		rptline ( &ReportOptions, ColumnArray, ColumnCount, tokens, tokcnt );
	}

	nsFclose ( fpData );
	rptclose ( &ReportOptions );
}
