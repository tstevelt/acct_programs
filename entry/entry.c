/*----------------------------------------------------------------------------
	Program : entry.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting Enter/Edit Transactions

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
#include	"entry.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	long	CurTrx;
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	GetInput ();

	StartUp ( "accounting - entry", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", OnQuit[0] == 'I' ? GL_MENU : 0 );

	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
	LoadMember ( WhereClause, &xmember, 0 );

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "entry";
	ptrWebParms->WP_FormAction = "entry.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	shs_seed_random ();

	printf ( "<input type=hidden id='OnQuit' name='OnQuit' value='%c'>\n", OnQuit[0] );
	printf ( "<input type=hidden id='what' name='what' value='lunch'>\n" );

	switch ( RunMode )
	{
		case MODE_START:
			PaintTrx ();
			break;

		case MODE_FIND:
			if ( xtrxh.xtrxnum == 0 )
			{
				ChkInput ();
				if ( AcctErrorCount == 0 )
				{
					if ( Lookup () > 0 )
					{
						break;
					}
				}
			}
			CurTrx = xtrxh.xtrxnum;
			RunMode = MODE_START;
			memset ( TrxArray, '\0', sizeof(TrxArray) );
			TrxCount = 0;
			xtrxh.xtrxnum = CurTrx;
			PaintTrx ();
			break;

		case MODE_PREV:
#ifdef OLD_STUFF
			if (( CurTrx = xtrxh.xtrxnum - 1 ) > 0 )
#endif
			CurTrx = FindTrx ( xtrxh.xtrxnum, -1 );
			if ( CurTrx != xtrxh.xtrxnum )
			{
				memset ( TrxArray, '\0', sizeof(TrxArray) );
				xtrxh.xtrxnum = CurTrx;
			}
			RunMode = MODE_START;
			PaintTrx ();
			break;

		case MODE_NEXT:
#ifdef OLD_STUFF
			if (( CurTrx = xtrxh.xtrxnum + 1 ) > 0 )
#endif
			CurTrx = FindTrx ( xtrxh.xtrxnum, 1 );
			if ( CurTrx != xtrxh.xtrxnum )
			{
				memset ( TrxArray, '\0', sizeof(TrxArray) );
				xtrxh.xtrxnum = CurTrx;
			}
			RunMode = MODE_START;
			PaintTrx ();
			break;

		case MODE_CLEAR:
			memset ( &xtrxh, '\0', sizeof(xtrxh) );
			memset ( TrxArray, '\0', sizeof(TrxArray) );
			TrxCount = 0;
			RunMode = MODE_START;
			PaintTrx ();
			break;

		case MODE_SAVE: 
			ChkInput ();
			if ( AcctErrorCount == 0 )
			{
				SaveTrx ();
			}
			RunMode = MODE_START;
			PaintTrx ();
			break;

		case MODE_LOOKUP:
			/*--------------------------------------------------------------
				this paints in a new window.
			--------------------------------------------------------------*/
			PaintLookup ();
			break;

		case MODE_DELETE: 
			ChkInput ();
			if ( AcctErrorCount == 0 )
			{
				if (DeleteTrx ( xtrxh.xtrxnum ) == 0 )
				{
					memset ( TrxArray, '\0', sizeof(TrxArray) );
					TrxCount = 0;
					memset ( &xtrxh, '\0', sizeof(XTRXH) );
					memset ( &xtrxd, '\0', sizeof(XTRXD) );
					RunMode = MODE_START;
				}
			}
			PaintTrx ();
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
