/*----------------------------------------------------------------------------
	Program : impcoa.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting Import Chart of Accounts

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
#include	"impcoa.h"

int		UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	int		ByteCount;
	char	tempfn[80];
	FILE	*tempfp;
	int		Char;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	RunMode = MODE_START;

	ByteCount = 0;
	sprintf ( tempfn, "/var/local/tmp/coa_%d.txt", getpid() );
	if (( tempfp = fopen ( tempfn, "w" )) == (FILE *) 0 )
	{
		printf ( "can not create temp file [%s] for storage<br>\n", tempfn );
	}
	else
	{
		while (( Char = fgetc ( stdin )) != EOF )
		{
			fputc ( Char, tempfp );
			ByteCount++;
		}
		nsFclose ( tempfp );
	}

	if ( ByteCount == 0 )
	{
		RunMode = MODE_START;
	}
	else
	{
		RunMode = MODE_IMPORT;
	}

	switch ( RunMode )
	{
		case MODE_START:
			StartUp ( "accounting - impcoa", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", SYS_MENU );

			snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
			LoadMember ( WhereClause, &xmember, 0 );

			printf ( "<form name='impform' id='impform' action='impcoa.cgi' method='POST'" );
			printf ( " enctype='multipart/form-data'>\n" );
			PaintScreen ();
			break;

		case MODE_IMPORT:
			StartUp ( "accounting - impcoa", SessionRecord.DbName, 1, 0, 0, "javascript:Loaded();", SYS_MENU );
			DoFile ( tempfn );
			break;
	}

	printf ( "</form>\n" );

//	unlink ( tempfn );

	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	return ( 0 );
}
