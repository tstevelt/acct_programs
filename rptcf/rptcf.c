/*----------------------------------------------------------------------------
	Program : rptcf.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Cash Flow Report

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"rptcf.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	GetInput ();

	switch ( RunMode  )
	{
		case MODE_START:
			StartUp ( "accounting - cash flow report", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );

			ptrWebParms = webInitParms ();

			ptrWebParms->WP_FormName   = "rptform";
			ptrWebParms->WP_FormAction = "rptcf.cgi";
			ptrWebParms->WP_FormMethod = "POST";
			ptrWebParms->WP_FormOther  = (char *)0;
			webStartForm ( ptrWebParms );

			webFreeParms ( ptrWebParms );

			printf ( "<input type=hidden name='what' value='lunch'>\n" );

			break;

		case MODE_RUN:
			ReportUp ( "cash flow report", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "cashflow" );
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
						fprintf ( fpBody, "Cash Flow Report\n" );
						fprintf ( fpBody, "%s\n", xsystem.xname );
						fprintf ( fpBody, "from %02d/%02d/%04d to %02d/%02d/%04d\n",
								prevdate.month, prevdate.day, prevdate.year4,
								currdate.month, currdate.day, currdate.year4 );
						nsFclose ( fpBody );

snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
LoadMember ( WhereClause, &xmember, 0 );
rptdeliv ( xmember.xmemail, xmember.xmemail, "cash flow", fnBody, ReportOptions.OutputFilename, ReportOptions.Format, 0 );

						unlink ( fnBody );
						break;
			 
					case RPT_FORMAT_HTML:
						rptcat ( ReportOptions.OutputFilename );
						
				}
			 
				unlink ( fnData );
				unlink ( ReportOptions.OutputFilename );
													
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
