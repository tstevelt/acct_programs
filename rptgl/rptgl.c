/*----------------------------------------------------------------------------
	Program : rptgl.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Print either Balance Sheet or Income Statement

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"rptgl.h"

int	UseErrorSys = 1;
static	int		DebugMain = 0;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	GetInput ();

	switch ( RunMode  )
	{
		case MODE_START:
			if ( CategoryType == 'I' )
			{
				StartUp ( "accounting - income statement", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}
			else
			{
				StartUp ( "accounting - balance sheet", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}

			ptrWebParms = webInitParms ();
		
			ptrWebParms->WP_FormName   = "rptform";
			ptrWebParms->WP_FormAction = "rptgl.cgi";
			ptrWebParms->WP_FormMethod = "POST";
			ptrWebParms->WP_FormOther  = (char *)0;
			webStartForm ( ptrWebParms );

			webFreeParms ( ptrWebParms );

			printf ( "<input type=hidden name='type' value='%c'>\n", CategoryType );
			printf ( "<input type=hidden name='what' value='lunch'>\n" );

			break;

		case MODE_RUN:
			if ( CategoryType == 'I' )
			{
				ReportUp ( "income statement", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "income" );
			}
			else
			{
				ReportUp ( "balance sheet", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "balance" );
			}
			break;
	}

	switch ( RunMode )
	{
		case MODE_START:
			PaintScreen ();
			printf ( "</form>\n" );
			break;

		case MODE_RUN:
			if ( ChkInput () != 0 )
			{
				break;
			}
#ifdef OLDSTUFF
			printf ( "<pre>\n" );
			if ( xsystem.xprintdest[0] == 'L' )
			{
				Report ( stdout );
			}
			else
			{
				FILE	*fp;

				sprintf ( FileName, "/tmp/rptgl_%d.txt", getpid () );
				if (( fpData = fopen ( FileName, "w" )) == (FILE *) 0 )
				{
					SaveError ( "can not create report file" );
					break;
				}
				Report ( fp );
				nsFclose ( fp );
				sprintf ( StatementOne, "lp -s %s > %s 2>&1", FileName, LOGFILENAME );
				system ( StatementOne );
				if ( DebugMain == 0 )
				{
					unlink ( FileName );
				}
				printf ( "okay<br>\n" );
			}
			printf ( "</pre>\n" );
#endif
			if ( getdata () > 0 )
			{
				dorpt ();

				switch ( ReportOptions.Format )
				{
					case RPT_FORMAT_CSV:
					case RPT_FORMAT_EXCEL:
					case RPT_FORMAT_TEXT:
					case RPT_FORMAT_PDF_EMAIL:
					case RPT_FORMAT_PDF_VIEW:
						sprintf ( fnBody, "%s/body_%d.txt", TEMPDIR, getpid() );
						if (( fpBody = fopen ( fnBody, "w" )) == (FILE *)0 )
						{
							printf ( "Cannot create %s for output\n", fnBody );
							break;
						}
						if ( CategoryType == 'I' )
						{
							fprintf ( fpBody, "Income Statement\n" );
						}
						else
						{
							fprintf ( fpBody, "Balance Sheet\n" );
						}
// fixit						fprintf ( fpBody, "%s\n", xsystem.xname );
// fixit						fprintf ( fpBody, "%02d/%02d/%04d - %02d/%02d/%04d\n",
// fixit								startdate.month, startdate.day, startdate.year4,
// fixit								enddate.month, enddate.day, enddate.year4 );
						nsFclose ( fpBody );

snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
LoadMember ( WhereClause, &xmember, 0 );
rptdeliv ( xmember.xmemail, xmember.xmemail, "rptgl", fnBody, ReportOptions.OutputFilename, ReportOptions.Format, 0 );

						if ( DebugMain == 0 )
						{
							unlink ( fnBody );
						}
						break;
			 
					case RPT_FORMAT_HTML:
						rptcat ( ReportOptions.OutputFilename );
						
				}
			 
				if ( DebugMain == 0 )
				{
					unlink ( fnData );
					unlink ( ReportOptions.OutputFilename );
				}
													
			}
			break;

		default :
			sprintf ( StatementOne, "RunMode %d not finished.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	dbyClose ( &MySql );

	return ( 0 );
}
