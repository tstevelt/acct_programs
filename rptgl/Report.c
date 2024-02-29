/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create report
	Return  : 
------------------------------------------------------------------------------*/
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

#include	"rptgl.h"

static	int		DebugReport = 0;
static	long	ReportTotal;


static int ReportType ( char AccountType, long *TypeTotal  )
{
	char		Fragment[40];
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	long		open_balance;
	long		account_total;
	long		type_total;
	int			Count;

	*TypeTotal = 0L;

	sprintf ( StatementOne, "select %s from account where accttype = '%c' order by acctnum",
				FIELDS_ACCOUNT, AccountType );

	if ( DebugReport )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	type_total= 0L;
	Count = 0;

	QueryOne = dbySelect ( "rptgl", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		open_balance = safe_atol ( QueryOne->EachRow[ACCOUNT_openbal] );

		sprintf ( StatementTwo,
			"select sum(amount) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate <= '%04d-%02d-%02d'",
				QueryOne->EachRow[ACCOUNT_acctnum],
				enddate.year4, enddate.month, enddate.day );

		if ( CategoryType == 'I' )
		{
			sprintf ( Fragment,
				" and trxdate >= '%04d-%02d-%02d'",
					startdate.year4,startdate.month, startdate.day );

			strcat ( StatementTwo, Fragment );
		}

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptgl", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			account_total = 0L;
		}
		else
		{
			account_total = safe_atol ( QueryTwo->EachRow[0] );
		}

		if ( CategoryType == 'B' )
		{
			account_total += open_balance;
		}

		if ( IncludeZeros == 'N' && account_total == 0L )
		{
			continue;
		}

		switch ( AccountType )
		{
			case 'A': fprintf ( fpData, "ASSET|" ); break;
			case 'L': fprintf ( fpData, "LIABILITY|" ); break;
			case 'E': fprintf ( fpData, "EQUITY|" ); break;
			case 'I': fprintf ( fpData, "INCOME|" ); break;
			case 'X': fprintf ( fpData, "EXPENSE|" ); break;
		}

		fprintf ( fpData, "%-4.4s|%-30.30s|%9.2f\n",
				QueryOne->EachRow[ACCOUNT_acctnum], 
				QueryOne->EachRow[ACCOUNT_acctname],
				(double) account_total / 100.0 );
		Count++;

		type_total += account_total;

	}

	*TypeTotal = type_total;

	return ( Count );
}

int getdata ()
{
	int		lineno = 0;
	long	Asset, Liability, Equity;
	long	Income, Expense;

	sprintf ( fnData, "%s/rptgl_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	if ( CategoryType == 'B' )
	{
		lineno += ReportType ( 'A', &Asset );
		lineno += ReportType ( 'L', &Liability );
		lineno += ReportType ( 'E', &Equity );

		ReportTotal = Asset - Liability - Equity;
	}
	else
	{
		lineno += ReportType ( 'I', &Income );
		lineno += ReportType ( 'X', &Expense );

		ReportTotal = Income - Expense;
	}

	nsFclose ( fpData );

	return ( lineno );
}


static	COLUMN_HEADINGS	ColumnArray [] = 
{
	{ "account",	"",			INIT_STRING_LEFT },
	{ "name",		"",			INIT_STRING_LEFT },
	{ "amount",		"",			INIT_DOUBLE_RIGHT_TOTAL },
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

	if ( CategoryType == 'I' )
	{
		sprintf ( Title, "income statement - %s", xsystem.xname );

		sprintf ( SubTitle, "%02d/%02d/%04d - %02d/%02d/%04d",
				startdate.month, startdate.day, startdate.year4,
				enddate.month, enddate.day, enddate.year4 );
	}
	else
	{
		sprintf ( Title, "balance sheet - %s", xsystem.xname );

		sprintf ( SubTitle, "%02d/%02d/%04d",
				enddate.month, enddate.day, enddate.year4 );
	}

	ReportOptions.HeaderRows = 1;
	ReportOptions.Title = Title;
	ReportOptions.Subtitle = SubTitle;
	ReportOptions.BreakCount = 1;
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
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rptgl", getpid() );
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

	rpttotal ( &ReportOptions, ColumnArray, ColumnCount, 0 );
	ColumnArray[2].Total.doubleTotal[1] = (double) ReportTotal / 100.0;
	rpttotal ( &ReportOptions, ColumnArray, ColumnCount, 1 );

	rptclose ( &ReportOptions );

	nsFclose ( fpData );
}
