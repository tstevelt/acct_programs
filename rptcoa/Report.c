/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create report
	Return  : 
----------------------------------------------------------------------------*/

#include	"rptcoa.h"

static	int		DebugReport = 0;

int getdata ()
{
	int			lineno = 0;
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	long		transaction_count;

	sprintf ( fnData, "%s/rptcoa_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	sprintf ( StatementOne, "select %s from account order by acctnum", FIELDS_ACCOUNT );

	if ( DebugReport )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	QueryOne = dbySelect ( "rptcoa", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		sprintf ( StatementTwo,
			"select count(*), max(trxdate) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s",
				QueryOne->EachRow[ACCOUNT_acctnum] );

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptcoa", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			transaction_count= 0L;
		}
		else
		{
			transaction_count= safe_atol ( QueryTwo->EachRow[0] );
		}

		switch ( QueryOne->EachRow[ACCOUNT_accttype][0] )
		{
			case 'A': fprintf ( fpData, "ASSET|" ); break;
			case 'L': fprintf ( fpData, "LIABILITY|" ); break;
			case 'E': fprintf ( fpData, "EQUITY|" ); break;
			case 'I': fprintf ( fpData, "INCOME|" ); break;
			case 'X': fprintf ( fpData, "EXPENSE|" ); break;
			default : fprintf ( fpData, "UNKNOWN|" ); break;
		}

		fprintf ( fpData, "%s|%-30.30s|%ld|%s",
					QueryOne->EachRow[ACCOUNT_acctnum], 
					QueryOne->EachRow[ACCOUNT_acctname],
					transaction_count,
					transaction_count > 0 ? QueryTwo->EachRow[1] : " " );
		
#ifdef NOT_NEEDED
		switch ( QueryOne->EachRow[ACCOUNT_accttype][0] )
		{
			case 'A': fprintf ( fpData, "1\n" ); break;
			case 'L': fprintf ( fpData, "2\n" ); break;
			case 'E': fprintf ( fpData, "3\n" ); break;
			case 'I': fprintf ( fpData, "4\n" ); break;
			case 'X': fprintf ( fpData, "5\n" ); break;
			default : fprintf ( fpData, "9\n" ); break;
		}
#else
		fprintf ( fpData, "\n" );
#endif
		lineno++;

		if ( DebugReport )
		{
			printf ( "transaction_count %ld max date %s<br>\n", transaction_count, QueryTwo->EachRow[1]  );
		}
	}

	nsFclose ( fpData );

	return ( lineno );
}

static	COLUMN_HEADINGS	ColumnArray [] = 
{
	{ "account","",				INIT_STRING_LEFT },
	{ "name",	"",				INIT_STRING_LEFT },
	{ "transaction", "count",	INIT_STRING_LEFT },
	{ "latest",	"transaction",	INIT_STRING_LEFT },
};

static  int		ColumnCount = sizeof(ColumnArray) / sizeof(COLUMN_HEADINGS);

void dorpt ()
{
	char	xbuffer[1024];
#define		MAXTOKS		10
	char	*tokens[MAXTOKS];
	int		tokcnt;
	char	Title[80];

	sprintf ( Title, "chart of accounts - %s", xsystem.xname );

	ReportOptions.HeaderRows = 1;
	ReportOptions.Title = Title;
	ReportOptions.Subtitle = "";
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
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rptcoa", getpid() );
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
