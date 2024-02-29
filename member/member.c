/*----------------------------------------------------------------------------
	Program : member.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Member maintenance screen

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#define		MAIN
#include	"member.h"

int	UseErrorSys = 1;
static	int		DebugMain = 0;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "member - member", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "member";
	ptrWebParms->WP_FormAction = "member.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );
	
	if ( DebugMain )
	{
		printf ( "RunMode %d, xid %ld<br>\n", RunMode, xmember.xid );
	}

	/*----------------------------------------------------------
		ChkInput may change RunMode depending on errors or input.
	----------------------------------------------------------*/
	ChkInput ();

	switch ( RunMode )
	{
		case MODE_QUIT:
			memset ( &xmember, '\0', sizeof(xmember) );
			RunMode = MODE_START;
			/* fall-through */
		case MODE_START:
			PaintScreen ();
			break;

		case MODE_LOAD:
			Load ();
			PaintScreen ();
			break;

		case MODE_LOOKUP:
			if ( Lookup () < 1 )
			{
				RunMode = MODE_START;
				PaintScreen ();
			}
			break;

		case MODE_INSERT:
			Insert ();
			RunMode = MODE_FOUND;
			PaintScreen ();
			break;

		case MODE_UPDATE:
			Update ();
			RunMode = MODE_FOUND;
			PaintScreen ();
			break;

		case MODE_DELETE:
			if ( Delete () == 0 )
			{
				memset ( &xmember, '\0', sizeof(xmember) );
			}
			PaintScreen ();
			break;

		case MODE_FOUND:
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
