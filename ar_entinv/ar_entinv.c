/*----------------------------------------------------------------------------
	Program : ar_entinv.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: A/R Invoice Entry

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
#include	"ar_entinv.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;
	int			rv, GivenInvoice;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - ar_entinv", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", AR_MENU );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "ar_entinv";
	ptrWebParms->WP_FormAction = "ar_entinv.cgi";
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
			if ( xarinvh.xid > 0 )
			{
				GivenInvoice = 1;
			}
			else
			{
				GivenInvoice = 0;
			}
			rv = Find ();
			if ( GivenInvoice )
			{
				PaintScreen ();
			}
			else if ( rv < 1 )
			{
				PaintScreen ();
			}
			break;

		case MODE_ADD_HEAD:
			AddHead ();
			PaintScreen ();
			break;

		case MODE_ADD_LINE:
			AddLine ();
			snprintf ( WhereClause, sizeof(WhereClause), "arinvh.id = %ld", xarinvh.xid );
			LoadArinvh ( WhereClause, &xarinvh, 0 );
			snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
			LoadArcust ( WhereClause, &xarcust, 0 );
			PaintScreen ();
			break;

		case MODE_SAVE:
			Save ();
			snprintf ( WhereClause, sizeof(WhereClause), "arinvh.id = %ld", xarinvh.xid );
			LoadArinvh ( WhereClause, &xarinvh, 0 );
			snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
			LoadArcust ( WhereClause, &xarcust, 0 );
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
