/*----------------------------------------------------------------------------
	Program : ImportOFX.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Import OFX type file
	Return  : 
----------------------------------------------------------------------------*/

#include	"import.h"

static int FindEnd ( char Buffer[] )
{
	int		Length;

	for ( Length = 0; ; Length++ )
	{
		switch ( Buffer[Length] )
		{
			case '\n':
			case '\r':
			case '\0':
			case '<':
				return ( Length );
			default:
				break;
		}
	}

	return ( Length );
}

int ImportOFX ( char *Filename )
{
	FILE	*fp;
	char	*BigBuffer = NULL;
	int		BufferSize = 0;
	long	FileSize;
	int		InTransaction = 0;
	int		OutputCount = 0;
	int		ndx, nr, DataLength;

	if ( Verbose )
	{
		printf ( "Starting ImportOFX\n" );
	}

/*----------------------------------------------------------
	may have very long lines
	may not have closing tags.
	<STMTTRN>
		... data in here.
	</STMTTRN>
----------------------------------------------------------*/
	if (( fp = fopen ( Filename, "r" )) == (FILE *)0 )
	{
		return ( -1 );
	}

	fseek ( fp, 0, SEEK_END );
	FileSize = ftell ( fp );
	rewind ( fp );

	BufferSize = FileSize;
	if (( BigBuffer = malloc ( BufferSize )) == NULL )
	{
		printf ( "Cannot malloc BigBuffer\n" );
		return ( -1 );
	}

	if (( nr = fread ( BigBuffer, 1, BufferSize, fp )) != BufferSize )
	{
		printf ( "Cannot read %d from %s\n", BufferSize, Filename );
		return ( -3 );
	}

	nsFclose ( fp );

	if ( Verbose )
	{
		printf ( "File size %ld, read %d\n", FileSize, nr );
	}

	DataLength = 0;
	for ( ndx = 0; ndx < BufferSize; ndx++ )
	{
		if ( nsStrncmp ( &BigBuffer[ndx], "<STMTTRN>", 9 ) == 0 )
		{
			InTransaction = 1;
			ndx += 8;
		}
		else if ( nsStrncmp ( &BigBuffer[ndx], "</STMTTRN>", 10 ) == 0 )
		{
			if ( InTransaction )
			{
				if ( Verbose )
				{
					printf ( "Save %04d-%02d-%02d [%s] [%s] %ld\n",
						NewTrxh.xtrxdate.year4, NewTrxh.xtrxdate.month, NewTrxh.xtrxdate.day,
						NewTrxh.xrefnum, NewTrxd.xpayee, NewTrxd.xamount );
				}
				SaveTransaction ();
				OutputCount++;
			}

			InTransaction = 0;
			ndx += 9;
		}

		/*----------------------------------------------------------
			<STMTTRN>
			<TRNTYPE>DEBIT
			<DTPOSTED>20201207120000
			<DTUSER>20201207120000
			<TRNAMT>-32.72
			<FITID>4005460549202012071
			<NAME>KROGER #741 52 MADISON IN
			<MEMO>POS PURCHASE POS99999999 3990258
			</STMTTRN>
		----------------------------------------------------------*/
		else if ( nsStrncmp ( &BigBuffer[ndx], "<TRNTYPE>", 9 ) == 0 )
		{
			ndx += 9;
		}
		else if ( nsStrncmp ( &BigBuffer[ndx], "<DTPOSTED>", 10 ) == 0 )
		{
			ndx += 10;
			DataLength = FindEnd ( &BigBuffer[ndx] );
			if ( DataLength > 8 )
			{
				BigBuffer[ndx+8] = '\0';
			}
			else
			{
				BigBuffer[ndx+DataLength] = '\0';
			}
			StrToDatevalFmt ( &BigBuffer[ndx], DATEFMT_YYYYMMDD, &NewTrxh.xtrxdate );
		}
		else if ( nsStrncmp ( &BigBuffer[ndx], "<TRNAMT>", 8 ) == 0 )
		{
			double	dblAmount;

			ndx += 8;
			dblAmount = nsAtof ( &BigBuffer[ndx] ) * 100.0;
			if ( dblAmount > 0.0 )
			{
				NewTrxd.xamount = (long)(0.49 + dblAmount);
			}
			else
			{
				NewTrxd.xamount = (long)(dblAmount - 0.49);
			}
		}
		else if ( nsStrncmp ( &BigBuffer[ndx], "<NAME>", 6 ) == 0 )
		{
			ndx += 6;
			DataLength = FindEnd ( &BigBuffer[ndx] );
			snprintf ( NewTrxd.xpayee, sizeof(NewTrxd.xpayee), "%*.*s", DataLength, DataLength, &BigBuffer[ndx] );
		}
		else if ( nsStrncmp ( &BigBuffer[ndx], "<MEMO>", 6 ) == 0 )
		{
			ndx += 6;
			DataLength = FindEnd ( &BigBuffer[ndx] );
			snprintf ( NewTrxh.xrefnum, sizeof(NewTrxh.xrefnum), "%*.*s", DataLength, DataLength, &BigBuffer[ndx] );
		}

	}

	free ( BigBuffer );

	if ( Verbose )
	{
		printf ( "Saved %d\n", OutputCount );
	}

	return ( 0 );
}
