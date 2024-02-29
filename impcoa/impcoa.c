/*----------------------------------------------------------------------------
	Program : impcoa.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Accounting Import Chart of Accounts

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/



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
