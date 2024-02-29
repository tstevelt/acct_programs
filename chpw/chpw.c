/*----------------------------------------------------------------------------
	Program : chpw.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting change password

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/


#define		MAIN
#include	"chpw.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

	GetInput ();

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, NULL, 0 );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - chpw", SessionRecord.DbName, 0, 1, 1, "javascript:Loaded();", SYS_MENU );

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "chpw";
	ptrWebParms->WP_FormAction = "chpw.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	switch ( RunMode )
	{
		case MODE_PAINT_FORGOT:
			if ( ChkInput () != 0 )
			{
				RunMode = MODE_PAINT_CHANGE;
			}
		case MODE_PAINT_CHANGE:
			PaintScreen ();
			break;

		case MODE_SAVE_CHANGE:
			if ( ChkInput () == 0 )
			{
				if ( ProcessChange () == 0 )
				{
					SaveError ( "okay" );
				}
				else
				{
					SaveError ( "new password not saved" );
					RunMode = MODE_PAINT_CHANGE;
				}
				break;
			}
			else
			{
				RunMode = MODE_PAINT_CHANGE;
			}
			PaintScreen ();
			break;

		case MODE_SAVE_FORGOT:
			if ( ChkInput () == 0 )
			{
				if ( ProcessForgot () == 0 )
				{
					SaveError ( "okay" );
				}
				else
				{
					SaveError ( "new password not saved" );
					RunMode = MODE_PAINT_FORGOT;
				}
				break;
			}
			else
			{
				RunMode = MODE_PAINT_FORGOT;
			}
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
