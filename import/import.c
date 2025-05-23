/*----------------------------------------------------------------------------
	Program : import.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Import transactions from external financial institutions
	Return  : 

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		Jan 2021	Auto-recognize file format.
	tms		04/03/2021	FixFidelity.sh (on iMac) and payee and refnum here.
	tms		01/09/2025	Added IGNORE words.  Initially hard coded.

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
#include	"import.h"

PARSEFUNC	ParseArray[] =
{
	{ "ParseChaseCC",	ParseChaseCC },
	{ "ParseNatCityCC",	ParseNatCityCC },
	{ "ParsePNC",		ParsePNC },
};

int		ParseCount = sizeof(ParseArray) / sizeof(PARSEFUNC);

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
	sprintf ( tempfn, "%s/import_%d.txt", TEMPDIR, getpid() );
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
			StartUp ( "accounting - import", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_MENU );

			snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
			LoadMember ( WhereClause, &xmember, 0 );

			printf ( "<form name='impform' id='impform' action='import.cgi' method='POST'" );
			printf ( " enctype='multipart/form-data'>\n" );
			PaintScreen ();
			printf ( "</form>\n" );
			break;

		case MODE_IMPORT:
			StartUp ( "accounting - import", SessionRecord.DbName, 1, 0, 0, "javascript:Loaded();", 0 );
			DoFile ( tempfn );
			break;
	}

	if ( Verbose == 1 && RunMode == MODE_IMPORT )
	{
		printf ( "not removing %s<br>\n", tempfn );
	}
	else
	{
//		unlink ( tempfn );
	}

	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );

	return ( 0 );
}
