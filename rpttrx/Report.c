/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create the report file.
	Return  : 
----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
                         income report - mm/dd/yyyy - mm/dd/yyyy
                         expense report - mm/dd/yyyy - mm/dd/yyyy
						 custom title  - mm/dd/yyyy - mm/dd/yyyy
						 tom stevelt
						 p. o. box 37622
						 cincinnati, oh 45222

account 
     trx        date   refnum   payee/memo                            amount
----------------------------------------------------------------------------
9999 x30xxxxxxxxxxxxxxxxxxxxxxxxxxx
     99999999 mm/dd/yy x8xxxxxx x34/40xxxxxxxxxxxxxxxxxxxxxxxxxxxx 999999.99
123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
------------------------------------------------------------------------------*/

#include	"rpttrx.h"

static	int		DebugReport = 0;

#define	DEBUG_2
#undef	DEBUG_2

int getdata ()
{
	int		lineno = 0;
	char		Fragment[20];
	int			xi;
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	DBY_QUERY	*QueryThree;
	long		grand_total;

	sprintf ( fnData, "%s/rpttrx_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	sprintf ( StatementOne, "select %s from account where acctnum in (", FIELDS_ACCOUNT );

	for ( xi = 0; xi < Count; xi++ )
	{
		if ( xi > 0 )
		{
			strcat ( StatementOne, "," );
		}

		sprintf ( Fragment, "%d", Array[xi] );
		strcat ( StatementOne, Fragment );
	}

	strcat ( StatementOne, ") order by acctnum" );

	if ( DebugReport == 1 )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	grand_total= 0L;

	QueryOne = dbySelect ( "rpttrx", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		sprintf ( StatementTwo,
"select %s from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate >= '%04d-%02d-%02d' and trxdate <= '%04d-%02d-%02d' order by trxdate",
				FLDS_TRXV,
				QueryOne->EachRow[ACCOUNT_acctnum],
				xrpt.xstartdate.year4, xrpt.xstartdate.month, xrpt.xstartdate.day,
				xrpt.xenddate.year4, xrpt.xenddate.month, xrpt.xenddate.day );

		QueryTwo = dbySelect ( "rpttrx", &MySql, StatementTwo, LOGFILENAME );

/*
     trx        date   refnum   payee/memo                            amount
     99999999 mm/dd/yy x8xxxxxx x34/40xxxxxxxxxxxxxxxxxxxxxxxxxxxx 999999.99
123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-
*/
		while (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
		{
			sprintf ( StatementThree,
				"select payee from trxd where seqnum = 1 and trxnum  = %s", 
					QueryTwo->EachRow[TRXV_trxnum] );

			QueryThree = dbySelect ( "rpttrx", &MySql, StatementThree, LOGFILENAME );

			xtrxd.xamount = safe_atol ( QueryTwo->EachRow[TRXV_amount] );
			grand_total += xtrxd.xamount;

			StringToDateval ( QueryTwo->EachRow[TRXV_trxdate], &xtrxh.xtrxdate );

			lineno++;

#ifdef DEBUG_2
			fprintf ( fpData, "%d|", lineno );
#endif

			fprintf ( fpData, "%s|%s|",
					QueryOne->EachRow[ACCOUNT_acctnum], QueryOne->EachRow[ACCOUNT_acctname] );

			fprintf ( fpData, "%-8.8s|", QueryTwo->EachRow[TRXV_trxnum] );

			fprintf ( fpData, "%02d/%02d/%02d|",
				xtrxh.xtrxdate.month, xtrxh.xtrxdate.day, xtrxh.xtrxdate.year2 );

			fprintf ( fpData, "%-8.8s|", QueryTwo->EachRow[TRXV_refnum] );

			if (( QueryThree->EachRow = mysql_fetch_row ( QueryThree->Result )) != NULL )
			{
				fprintf ( fpData, "%-34.34s|", QueryThree->EachRow[0] );
			}
			else
			{
				fprintf ( fpData, "%-34.34s|", " " );
			}

			fprintf ( fpData, "%9.2f|", (double) xtrxd.xamount / 100.0 );

			fprintf ( fpData, "%9.2f\n", (double) grand_total / 100.0 );

		}
	}

	nsFclose ( fpData );

	return ( lineno );
}

static	COLUMN_HEADINGS	ColumnArray [] = 
{
#ifdef DEBUG_2
	{ "lineno",		"",		INIT_LONG_LEFT },
#endif
	{ "trx",		"",		INIT_LONG_LEFT },
	{ "date",		"",		INIT_STRING_LEFT },
	{ "refnum",		"",		INIT_STRING_LEFT },
	{ "payee/memo",	"",		INIT_STRING_LEFT },
	{ "amount",		"",		INIT_DOUBLE_RIGHT_TOTAL },
	{ "running",	"balance",		INIT_DOUBLE_RIGHT },
};

static  int		ColumnCount = sizeof(ColumnArray) / sizeof(COLUMN_HEADINGS);

void dorpt ( char *ReportType )
{
	char	xbuffer[1024];
#define		MAXTOKS		10
	char	*tokens[MAXTOKS];
	int		tokcnt;
	char	Title[80];
	char	SubTitle[80];
#ifdef DEBUG_2
	int		lineno = 0;
#endif

	sprintf ( Title, "%s - %s", ReportType, xsystem.xname );

	sprintf ( SubTitle, "%02d/%02d/%04d - %02d/%02d/%04d",
			xrpt.xstartdate.month, xrpt.xstartdate.day, xrpt.xstartdate.year4,
			xrpt.xenddate.month, xrpt.xenddate.day, xrpt.xenddate.year4 );

	ReportOptions.HeaderRows = 2;
	ReportOptions.Title = Title;
	ReportOptions.Subtitle = SubTitle;
	ReportOptions.BreakCount = 2;
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
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rpttrx", getpid() );
	strcat ( ReportOptions.OutputFilename, rptextension ( ReportOptions.Format ) );
	ReportOptions.IsStdout = 0;

	fpData = rptinit ( fnData, &ReportOptions, ColumnArray, ColumnCount );

	if ( DebugReport == 1 )
	{
		printf ( "fnData                  %s<br>\n", fnData );
		printf ( "ReportOptions.OutputFilename %s<br>\n", ReportOptions.OutputFilename );
		printf ( "File Pointer %s<br>\n", fpData == NULL ? "null" : "okay" );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fpData ) != (char *)0 )
	{
		tokcnt = GetTokensA ( xbuffer, "|\n\r", tokens, MAXTOKS );

		if ( DebugReport == 1 )
		{
			printf ( "tokcnt %d<br>\n", tokcnt );
		}

		rptline ( &ReportOptions, ColumnArray, ColumnCount, tokens, tokcnt );
#ifdef DEBUG_2
		lineno++;
#endif
	}

	rpttotal ( &ReportOptions, ColumnArray, ColumnCount, 0 );
	rpttotal ( &ReportOptions, ColumnArray, ColumnCount, 1 );

	rptclose ( &ReportOptions );

	nsFclose ( fpData );

}
