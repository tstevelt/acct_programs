/*----------------------------------------------------------------------------
	Program : PaintLookup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint accounts.
	Return  : 
----------------------------------------------------------------------------*/

#include	"entry.h"

void PaintLookup ()
{
	int		xa;
	char	CurType = '?';

	sprintf  ( StatementTwo, " acctname like '%s%%' ", xaccount.xacctname );
	LoadAccounts ( StatementTwo );

	printf ( "<table class='AppWide'>\n" );

	for ( xa = 0; xa < AcctsCount; xa++ )
	{
		printf ( "<tr>\n" );

		if ( CurType != AcctsArray[xa].xaccttype[0] )
		{
			CurType = AcctsArray[xa].xaccttype[0];

			printf ( "<tr>\n" );
			printf ( "<th colspan='2'>" );
			switch ( CurType )
			{
				case 'A': printf ( "asset" ); break;
				case 'L': printf ( "liability" ); break;
				case 'E': printf ( "equity" ); break;
				case 'I': printf ( "income" ); break;
				case 'X': printf ( "expense" ); break;
			}
			printf ( "</th>" );
			printf ( "</tr>\n" );
		}

		printf ( "<td>" );

		printf ( "<a href=\"#\" onClick='window.opener.document.entry.%s.value=\"%ld\";",
			TargetNumber, AcctsArray[xa].xacctnum );

		printf ( "window.opener.document.entry.%s.value=\"%s\";",
			TargetName, AcctsArray[xa].xacctname );
			
		printf ( "window.close();'>%ld</a>", AcctsArray[xa].xacctnum );

		printf ( "</td>\n" );

		printf ( "<td>" );
		printf ( "%s", AcctsArray[xa].xacctname );
		printf ( "</td>\n" );

		printf ( "</tr>\n" );
	}

	printf ( "</table>\n" );

}
