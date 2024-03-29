/*----------------------------------------------------------------------------
	Program : ar_arcust.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: A/R Customer Maintenance screen

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/
//     Accounting Programs
// 
//     Copyright (C)  2000-2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.


#define		MAIN
#include	"ar_arcust.h"

int	UseErrorSys = 1;
static	int		DebugMain = 0;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );
	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - ar_arcust", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", AR_MENU );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "ar_arcust";
	ptrWebParms->WP_FormAction = "ar_arcust.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );
	
	if ( DebugMain )
	{
		printf ( "RunMode %d, xid %ld<br>\n", RunMode, xarcust.xid );
	}

	/*----------------------------------------------------------
		ChkInput may change RunMode depending on errors or input.
	----------------------------------------------------------*/
	ChkInput ();

	switch ( RunMode )
	{
		case MODE_QUIT:
			memset ( &xarcust, '\0', sizeof(xarcust) );
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
				memset ( &xarcust, '\0', sizeof(xarcust) );
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
