/*----------------------------------------------------------------------------
	Program : reconcile.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Reconcile account

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#define		MAIN
#include	"reconcile.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - reconcile", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_MENU );

	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
	LoadMember ( WhereClause, &xmember, 0 );

	printf ( "<script language='JavaScript1.2' src='reconcile.js?version=0202022602'></script>\n" );

	GetInput ();

	ChkInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "reconcile";
	ptrWebParms->WP_FormAction = "reconcile.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<input type=hidden name='what' value='lunch'>\n" );

	switch ( RunMode )
	{
		case MODE_START:
			PaintTop ();
			break;

		case MODE_GET:
			PaintTop ();
			LoadOpen ();
			PaintOpen ( MODE_GET );
			break;

		case MODE_SAVE:
			Update ();
			RunMode = MODE_GET;
			PaintTop ();
			LoadOpen ();
			if ( PaintOpen ( MODE_SAVE ) == 1 )
			{
				SaveEndDate ();
			}
			break;

		default :
			sprintf ( StatementOne, "RunMode %d not finished.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	printf ( "</form>\n" );

	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	return ( 0 );
}
