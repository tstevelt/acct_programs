/*----------------------------------------------------------------------------
	Program : asetup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting System Setup

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
#include	"asetup.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );
//	StartUp ( "accounting - setup", AcctCookie, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - setup", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

	PageNumber = 1;

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "setup";
	ptrWebParms->WP_FormAction = "asetup.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<input type='hidden' name='what' value='lunch'>\n" );
	printf ( "<input type='hidden' name='page' value='%d'>\n", PageNumber );

	switch ( RunMode )
	{
		case MODE_START:
			PaintSys ();
			break;

		case MODE_SAVE:
			if ( ChkInput () == 0 )
			{
				SaveSys ();
			}
			RunMode = MODE_START;
			PaintSys ();
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

	dbyClose ( &MySql );

	return ( 0 );
}
