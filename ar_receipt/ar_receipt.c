/*----------------------------------------------------------------------------
	Program : ar_receipt.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: A/R Cash Receipt Entry

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#define		MAIN
#include	"ar_receipt.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - ar_receipt", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", AR_MENU );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "ar_receipt";
	ptrWebParms->WP_FormAction = "ar_receipt.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	ChkInput ();

	switch ( RunMode )
	{
		case MODE_START:
			PaintScreen ();
			break;

		case MODE_FIND:
			Find ();
			PaintScreen ();
			break;

		case MODE_SAVE:
			Save ();
			PaintScreen ();
			break;

		default :
			sprintf ( StatementOne, "Unknown RunMode %d.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	printf ( "</form>\n" );

	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	dbyClose ( &MySql );

	return ( 0 );
}
