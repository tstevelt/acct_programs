/*----------------------------------------------------------------------------
	Program : rptall.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Report all transactions for given date range.

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"rptall.h"

int	UseErrorSys = 1;
static	int		DebugMain = 0;

int main ( int argc, char *argv[] )
{
	char			*ReportType;
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	GetInput ();

	switch ( RunMode  )
	{
		case MODE_START:
		case MODE_FIND:
			StartUp ( "accounting - transaction list", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );

			ptrWebParms = webInitParms ();

			ptrWebParms->WP_FormName   = "rptform";
			ptrWebParms->WP_FormAction = "rptall.cgi";
			ptrWebParms->WP_FormMethod = "POST";
			ptrWebParms->WP_FormOther  = (char *)0;
			webStartForm ( ptrWebParms );

			webFreeParms ( ptrWebParms );

			printf ( "<input type=hidden name='type' value='%c'>\n", CategoryType );
			printf ( "<input type=hidden name='what' value='lunch'>\n" );

			break;

		case MODE_RUN:
			ReportUp ( "transaction list", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "asset" );
			ReportType = "transaction list";
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

			if ( getdata () > 0 )
			{
				dorpt ( ReportType );

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
						fprintf ( fpBody, "%s Report\n", ReportType );
						fprintf ( fpBody, "%s\n", xsystem.xname );
						fprintf ( fpBody, "%02d/%02d/%04d - %02d/%02d/%04d\n",
								xrpt.xstartdate.month, xrpt.xstartdate.day, xrpt.xstartdate.year4,
								xrpt.xenddate.month, xrpt.xenddate.day, xrpt.xenddate.year4 );
						nsFclose ( fpBody );

snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
LoadMember ( WhereClause, &xmember, 0 );
rptdeliv ( xmember.xmemail, xmember.xmemail, ReportType, fnBody, ReportOptions.OutputFilename, ReportOptions.Format, 0 );

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
			sprintf ( StatementOne, "Unknown RunMode %d.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );
	
	dbyClose ( &MySql );

	return ( 0 );
}
