/*----------------------------------------------------------------------------
	Program : close.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting close "period", trx before closing date.

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define		MAIN
#include	"close.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - close", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_MENU );

	GetInput ();


	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
	LoadMember ( WhereClause, &xmember, 0 );

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "closeform";
	ptrWebParms->WP_FormAction = "close.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<input type=hidden name='what' value='lunch'>\n" );

	switch ( RunMode )
	{
		case MODE_START:
			PaintScreen ();
			printf ( "</form>\n" );
			break;

		case MODE_RUN:
			if ( ChkInput () == 0 )
			{
				Process ();
			}
			PaintScreen ();
			printf ( "</form>\n" );
			break;

		default :
			sprintf ( StatementOne, "unknown runmode %d.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	return ( 0 );
}
