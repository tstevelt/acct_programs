/*----------------------------------------------------------------------------
	Program : accounts.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: G/L Accounts Maintenance

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
#include	"accounts.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	WEBPARMS		*ptrWebParms;
	char		WhereClause[128];

	ptrWebParms = webInitParms ();

	AcctGetCookieAndSession ( &SessionRecord );
	StartUp ( "accounting - accounts and categories", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_MENU );

	snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
	LoadMember ( WhereClause, &xmember, 0 );

	GetInput ();

	ptrWebParms->WP_FormName   = "accounts";
	ptrWebParms->WP_FormAction = "accounts.cgi";
	ptrWebParms->WP_FormMethod = "POST";
	ptrWebParms->WP_FormOther  = (char *)0;
	webStartForm ( ptrWebParms );

	webFreeParms ( ptrWebParms );

	printf ( "<input type=hidden name='ac' value='%c'>\n", AC_Mode );
	printf ( "<input type=hidden name='what' value='lunch'>\n" );

	if ( LoadSystem () == 0 )
	{
		sprintf ( StatementOne, "LoadSystem returned NULL." );
		SaveError ( StatementOne );
	}

	switch ( RunMode )
	{
		case MODE_START:
			PaintList ();
			break;

		case MODE_EDIT:
			PaintOne ();
			break;

		case MODE_ADD_ACCOUNT:
			PaintOne ();
			break;

		case MODE_SAVE_ADD:
			if ( AddAccount () == 1 )
			{
				RunMode = MODE_EDIT;
			}
			else
			{
				RunMode = MODE_ADD_ACCOUNT;
			}
			PaintOne ();
			break;

		case MODE_SAVE_EDIT:
			SaveAccount ();
			if ( ximport.xacctnum > 0 )
			{
				SaveImport ( ximport.xacctnum );
			}
			RunMode = MODE_EDIT;
			PaintOne ();
			break;

		case MODE_DELETE_ACCOUNT:
			if ( DeleteAccount ( xaccount.xacctnum ) == 1 )
			{
				if ( ximport.xacctnum > 0 )
				{
					DeleteImport ( ximport.xacctnum );
				}

				RunMode = MODE_START;
				PaintList ();
			}
			else
			{
				RunMode = MODE_EDIT;
				PaintOne ();
			}
			break;

		case MODE_DELETE_IMPORT:
			if ( ximport.xacctnum > 0 )
			{
				DeleteImport ( ximport.xacctnum );
			}
			RunMode = MODE_EDIT;
			PaintOne ();
			break;

		case MODE_REGISTER:
			Register ();
			break;

		case MODE_ADD_IMPORT:
			if ( AddImport ( xaccount.xacctnum ) != 1 )
			{
				SaveError ( "could not add import" );	
			}
			RunMode = MODE_EDIT;
			PaintOne ();
			break;

		default :
			sprintf ( StatementOne, "run mode %d not finished.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	printf ( "</form>\n" );

	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	return ( 0 );
}
