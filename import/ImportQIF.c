/*----------------------------------------------------------------------------
	Program : ImportQIF.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Import QIF file.
	Return  : 
----------------------------------------------------------------------------*/

#include	"import.h"

#define		SMALLBUFSZ		1024

int ImportQIF ( char *Filename )
{
	FILE	*fp;
	char	xbuffer[SMALLBUFSZ];
	int		lineno = 0;

	if ( Verbose )
	{
		printf ( "Starting ImportQIF<br>\n" );
	}

	if (( fp = fopen ( Filename, "r" )) == (FILE *)0 )
	{
		return ( -1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		if ( lineno == 1 )
		{
			if ( nsStrncmp ( xbuffer, "!Type:", 6 ) != 0 )
			{
				printf ( "This is not a QIF file\n" );
				return ( -1 );
			}

			continue;
		}

		switch ( xbuffer[0] )
		{
			case 'D':
				StrToDatevalFmt ( &xbuffer[1], DATEFMT_MM_DD_YYYY, &NewTrxh.xtrxdate );
				break;
			case 'T':
				// NewTrxd.xamount = 0.5 + nsAtof ( &xbuffer[1] ) * 100.0;
				NewTrxd.xamount = nsAtof ( &xbuffer[1] ) * 100.00005;
				break;
			case 'P':
				snprintf ( OriginalPayee, sizeof(OriginalPayee), "%s", &xbuffer[1] );
				snprintf ( NewTrxd.xpayee, sizeof(NewTrxd.xpayee), "%s", &xbuffer[1] );
				break;
			case 'M':
				snprintf ( OriginalRefnum, sizeof(OriginalRefnum), "%s", &xbuffer[1] );
				snprintf ( NewTrxh.xrefnum, sizeof(NewTrxh.xrefnum), "%s", &xbuffer[1] );
				break;
			case '^':
				if ( Verbose )
				{
					printf ( "Save %04d-%02d-%02d [%s] [%s] %ld\n",
						NewTrxh.xtrxdate.year4, NewTrxh.xtrxdate.month, NewTrxh.xtrxdate.day,
						NewTrxh.xrefnum, NewTrxd.xpayee, NewTrxd.xamount );
				}
				SaveTransaction ();
				break;
			case 'C':
			default:
				break;

		}
	}

	nsFclose ( fp );

	return ( 0 );
}
