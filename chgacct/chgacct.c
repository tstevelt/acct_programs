/*----------------------------------------------------------------------------
	Program : chgacct.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Change G/L account number

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#define		MAIN
#include	"chgacct.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - chgacct", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
	LoadMember ( WhereClause, &xmember, 0 );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "chgacct";
	ptrWebParms->WP_FormAction = "chgacct.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<input type='hidden' name='what' value='lunch'>\n" );

	switch ( RunMode )
	{
		case MODE_START:
			PaintScreen ();
			break;

		case MODE_CHECK:
			if ( ChkInput () != 0 )
			{
				RunMode = MODE_START;
			}
			PaintScreen ();
			break;

		case MODE_SAVE:
			ChangeAcctnum ();
			RunMode = MODE_START;
			CurrentAcctnum = NewAcctnum = 0;
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
