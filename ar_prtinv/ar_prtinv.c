/*----------------------------------------------------------------------------
	Program : ar_prtinv.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: A/R Print Invoice

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"ar_prtinv.h"

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
			StartUp ( "accounting - print invoice", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", AR_MENU );

			ptrWebParms = webInitParms ();

			ptrWebParms->WP_FormName   = "rptform";
			ptrWebParms->WP_FormAction = "ar_prtinv.cgi";
			ptrWebParms->WP_FormMethod = "POST";
			ptrWebParms->WP_FormOther  = (char *)0;
			webStartForm ( ptrWebParms );

			webFreeParms ( ptrWebParms );

			printf ( "<input type=hidden name='what' value='lunch'>\n" );

			break;

		case MODE_RUN:
			ReportUp ( "print invoice", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "chart" );
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

				if ( ReportFormat == RPT_FORMAT_HTML )
				{
					printf ( "<pre>\n" );
					rptcat ( fnData );
					printf ( "</pre>\n" );
				}
				else
				{
					char	*Attachments[2];
					char	Subject[128];

					snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
					LoadMember ( WhereClause, &xmember, 0 );

					sprintf ( Subject, "Invoice %ld for %s", InvoiceNumber, xarcust.xname );
					Attachments[0] = fnPDF;
					Attachments[1] = fnData;
					if ( SendAttached ( "tms@stevelt.com", xmember.xmemail, "", "", Subject, 0, NULL, 2, Attachments ) == 0 )
					{
						printf ( "Sending two attachments to %s<br>", xmember.xmemail );
					}
					else
					{
						printf ( "An error occurred while sending email<br>\n" );
					}

				}
						
//				unlink ( fnData );
													
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
