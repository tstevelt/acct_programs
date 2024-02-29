/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create report file.
	Return  : 
----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------

123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-

trx      date     refnum   status
seq      payee                                    account name                       credit     debit
----------------------------------------------------------------------------
99999999 mm/dd/yy x8xxxxxx status?
99999    x40xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx x30xxxxxxxxxxxxxxxxxxxxxxxxxxx  999999.99 999999.99

------------------------------------------------------------------------------*/

#include	"rptall.h"

static	int		DebugReport = 0;

#define	DEBUG_2
#undef	DEBUG_2

int getdata ()
{
	int		lineno = 0;
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;

	sprintf ( fnData, "%s/rptall_%d.data", TEMPDIR, getpid () );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	sprintf ( StatementOne, 
		"select %s from trxh where trxdate >= '%04d-%02d-%02d' and trxdate <= '%04d-%02d-%02d' order by trxdate",
				FIELDS_TRXH,
				xrpt.xstartdate.year4, xrpt.xstartdate.month, xrpt.xstartdate.day,
				xrpt.xenddate.year4, xrpt.xenddate.month, xrpt.xenddate.day );

	if ( DebugReport == 1 )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	QueryOne = dbySelect ( "rptall", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		sprintf ( StatementTwo,
"select trxnum,seqnum,trxd.acctnum,payee,amount,acctname,accttype \
from trxd, account \
where trxd.trxnum = %ld and trxd.acctnum = account.acctnum  order by seqnum",
				nsAtol(QueryOne->EachRow[TRXH_trxnum]) );

		if ( DebugReport == 1 )
		{
			printf ( "  %s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptall", &MySql, StatementTwo, LOGFILENAME );

		while (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
		{
	  
			lineno++;


			fprintf ( fpData, "%d|%s|%s|%s|",
					nsAtoi(QueryOne->EachRow[TRXH_trxnum]), 
					QueryOne->EachRow[TRXH_trxdate],
					QueryOne->EachRow[TRXH_refnum],
					StatusDescr(nsAtoi(QueryOne->EachRow[TRXH_status]),2) );

			fprintf ( fpData, "%8.8s|",   QueryTwo->EachRow[1] );
			fprintf ( fpData, "%-40.40s|", QueryTwo->EachRow[3] );
			fprintf ( fpData, "%-30.30s|", QueryTwo->EachRow[5] );

			xtrxd.xamount = safe_atol ( QueryTwo->EachRow[4] );
			switch ( QueryTwo->EachRow[6][0] )
			{
				case 'A':
				case 'X':
					fprintf ( fpData, "%9.2f| ", (double) xtrxd.xamount / 100.0 );
					break;
				case 'E':
				case 'L':
				case 'I':
					fprintf ( fpData, " |%9.2f", (double) xtrxd.xamount / 100.0 );
					break;
			}

			fprintf ( fpData, "\n" );
		}
	}

	nsFclose ( fpData );

	return ( lineno );
}

static	COLUMN_HEADINGS	ColumnArray [] = 
{
	{ "seqno",			"",		INIT_STRING_RIGHT },
	{ "payee",			"",		INIT_STRING_LEFT },
	{ "account name",	"",		INIT_STRING_LEFT },
	{ "debit",			"",		INIT_DOUBLE_RIGHT_TOTAL },
	{ "credit",			"",		INIT_DOUBLE_RIGHT_TOTAL },
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

	ReportOptions.HeaderRows = 1;
	ReportOptions.Title = Title;
	ReportOptions.Subtitle = SubTitle;
	ReportOptions.BreakCount = 4;
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
	sprintf ( ReportOptions.OutputFilename,  "%s/%s_%d", TEMPDIR, "rptall", getpid() );
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
