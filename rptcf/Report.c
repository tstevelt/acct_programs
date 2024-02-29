/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create report
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

#include	"rptcf.h"

static	int		DebugReport = 0;
static	long	PrevReportTotal;
static	long	CurrReportTotal;

static int ReportType ( char AccountType, long *PrevTypeTotal, long *CurrTypeTotal  )
{
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	long		open_balance;
	long		prev_total;
	long		curr_total;
	long		prev_type_total;
	long		curr_type_total;
	int			Count;

	*PrevTypeTotal = 0L;
	*CurrTypeTotal = 0L;

	sprintf ( StatementOne, "select %s from account where accttype = '%c' order by acctnum",
				FIELDS_ACCOUNT, AccountType );

	if ( DebugReport )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	prev_type_total= 0L;
	curr_type_total= 0L;
	Count = 0;

	QueryOne = dbySelect ( "rptcf", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		open_balance = safe_atol ( QueryOne->EachRow[ACCOUNT_openbal] );

		/*----------------------------------------------------------
			get current balance for account
		----------------------------------------------------------*/
		sprintf ( StatementTwo,
			"select sum(amount) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate <= '%04d-%02d-%02d'",
				QueryOne->EachRow[ACCOUNT_acctnum],
				currdate.year4, currdate.month, currdate.day );

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptcf", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			curr_total = 0L;
		}
		else
		{
			curr_total = safe_atol ( QueryTwo->EachRow[0] );
		}

		if ( CategoryType == 'B' )
		{
			curr_total += open_balance;
		}

		/*----------------------------------------------------------
			get previous balance for account
		----------------------------------------------------------*/
		sprintf ( StatementTwo,
			"select sum(amount) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate <= '%04d-%02d-%02d'",
				QueryOne->EachRow[ACCOUNT_acctnum],
				prevdate.year4, prevdate.month, prevdate.day );

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptcf", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			prev_total = 0L;
		}
		else
		{
			prev_total = safe_atol ( QueryTwo->EachRow[0] );
		}

		if ( CategoryType == 'B' )
		{
			prev_total += open_balance;
		}

		if ( IncludeZeros == 'N' && prev_total == 0L && curr_total == 0L )
		{
			continue;
		}

		switch ( AccountType )
		{
			case 'A': fprintf ( fpData, "ASSET|" ); break;
			case 'L': fprintf ( fpData, "LIABILITY|" ); break;
			case 'E': fprintf ( fpData, "EQUITY|" ); break;
		}

		fprintf ( fpData, "%-4.4s|%-30.30s|%9.2f|%9.2f|%9.2f\n",
				QueryOne->EachRow[ACCOUNT_acctnum], 
				QueryOne->EachRow[ACCOUNT_acctname],
				(double) prev_total / 100.0,
				(double) curr_total / 100.0,
				(double) (curr_total - prev_total) / 100.0 );
		Count++;

		prev_type_total += prev_total;
		curr_type_total += curr_total;

	}

	*PrevTypeTotal = prev_type_total;
	*CurrTypeTotal = curr_type_total;

	return ( Count );
}

int getdata ()
{
	int		lineno = 0;
	long	PrevAsset, PrevLiability, PrevEquity;
	long	CurrAsset, CurrLiability, CurrEquity;
//	long	PrevIncome, PrevExpense;
//	long	CurrIncome, CurrExpense;

	sprintf ( fnData, "%s/rptcf_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	if ( CategoryType == 'B' )
	{
		lineno += ReportType ( 'A', &PrevAsset, &CurrAsset );
		lineno += ReportType ( 'L', &PrevLiability, &CurrLiability );
		lineno += ReportType ( 'E', &PrevEquity, &CurrEquity );

		PrevReportTotal = PrevAsset - PrevLiability - PrevEquity;
		CurrReportTotal = CurrAsset - CurrLiability - CurrEquity;
	}
//	else
//	{
//		lineno += ReportType ( 'I', &PrevIncome, &CurrIncome );
//		lineno += ReportType ( 'X', &PrevExpense, &CurrExpense );
//
//		PrevReportTotal = PrevIncome - PrevExpense;
//		CurrReportTotal = CurrIncome - CurrExpense;
//	}

	nsFclose ( fpData );

	return ( lineno );
}


static	COLUMN_HEADINGS	ColumnArray [] = 
{
	{ "account",	"",			INIT_STRING_LEFT },
	{ "name",		"",			INIT_STRING_LEFT },
	{ "previous",	"amount",	INIT_DOUBLE_RIGHT_TOTAL },
	{ "current",	"amount",	INIT_DOUBLE_RIGHT_TOTAL },
	{ "cash",		"flow",		INIT_DOUBLE_RIGHT_TOTAL },
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

	if ( CategoryType == 'B' )
	{
		sprintf ( Title, "cash flow report - %s", xsystem.xname );

		sprintf ( SubTitle, "from %02d/%02d/%04d to %02d/%02d/%04d",
				prevdate.month, prevdate.day, prevdate.year4,
				currdate.month, currdate.day, currdate.year4 );
	}
//	else if ( CategoryType == 'I' )
//	{
//		sprintf ( Title, "income statement - %s", xsystem.xname );
//
//	}

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
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rptcf", getpid() );
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
	ColumnArray[2].Total.doubleTotal[1] = (double) PrevReportTotal / 100.0;
	ColumnArray[3].Total.doubleTotal[1] = (double) CurrReportTotal / 100.0;
	ColumnArray[4].Total.doubleTotal[1] = (double) (CurrReportTotal - PrevReportTotal) / 100.0;
	rpttotal ( &ReportOptions, ColumnArray, ColumnCount, 1 );

	rptclose ( &ReportOptions );

	nsFclose ( fpData );
}
