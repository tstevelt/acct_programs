/*----------------------------------------------------------------------------
	Program : backup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Main entry point for database backup program
	Return  : 
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
#include	"backup.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS	*ptrWebParms;

//	AcctCookie = acctGetCookie ( ACCT_COOKIE, COOKIE_FAIL, 0 );
	AcctGetCookieAndSession ( &SessionRecord );

	StartUp ( "accounting - backup", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

	GetInput ();

	ptrWebParms = webInitParms ();

	ptrWebParms->WP_FormName   = "backup";
	ptrWebParms->WP_FormAction = "backup.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	switch ( RunMode )
	{
		case MODE_START:
			PaintScreen ();
			break;

		case MODE_SAVE:
			if ( ChkInput () == 0 )
			{
				Process ();
			}
//			RunMode = MODE_START;
//			PaintScreen ();
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
