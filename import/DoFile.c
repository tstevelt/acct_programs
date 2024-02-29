/*----------------------------------------------------------------------------
	Program : DoFile.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Read the file and call approprite parsing function
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

#include	"import.h"

#define USE_POPUP 
#undef USE_POPUP 

#define	USE_PARSE_FUNCTIONS
#undef USE_PARSE_FUNCTIONS

#ifdef USE_PARSE_FUNCTIONS
extern	PARSEFUNC	ParseArray[];
extern	int			ParseCount;
#endif
extern	double		dblScore;

#define		CONTENT_BEFORE_FILE		6

static	char	*BigBuffer = NULL;
static	int		BufferSize = 0;
static	int		BufferCount = 0;

#define		SMALLBUFSZ		1024

int DoFile ( char *tempfn )
{
	FILE		*fp;
	char		xbuffer[SMALLBUFSZ];
	char		*zbuffer;
	int			ContentCount;
	int			lineno;
	int			DateClosed;
#ifdef USE_PARSE_FUNCTIONS
	int			(*Parse)();	
	int			xp;
#endif
	int			LineLength;
	SHS_FILE_TYPE	*ptrFileType;
	char		ScratchFile[256];
	FILE		*fpScratch;
	long		Affected;

	MaxDate.year2  = 0;
	MaxDate.year4  = 0;
	MaxDate.month = 0;
	MaxDate.day   = 0;

	ReportCount = DateCount = DateClosed = DatePrevious = 0;

	if ( BufferSize == 0 )
	{
		BufferSize = SMALLBUFSZ * 10;
		if (( BigBuffer = calloc ( BufferSize, 1 )) == NULL )
		{
			fprintf ( stderr, "Cannot calloc BigBuffer\n" );
			return ( -1 );
		}
	}

	/*--------------------------------------------------------------
		open the file
	--------------------------------------------------------------*/
	if (( fp = fopen ( tempfn, "r" )) == (FILE *)0 )
	{
		sprintf ( StatementOne, "Can not open file %s, errno %d", tempfn, errno );
		SaveError ( StatementOne );
		return ( -2 );
	}

	/*--------------------------------------------------------------
		read first part of file to find account number and 
		parse function.  break and go in to second while loop
		to read the actual data.
	--------------------------------------------------------------*/
	lineno = 0;
	ContentCount = 0;
	ImportAcctnum = 0;
	AutoStatus  = -1;
	Verbose     = -1;
	ReportSkips = -1;

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		if ( nsStrlen ( xbuffer ) == 0 )
		{
			continue;
		}
		else if ( nsStrncmp ( xbuffer, "--------", 8 ) == 0 )
		{
			continue;
		}
		else if ( nsStrncmp ( xbuffer, "Content-", 8 ) == 0 )
		{
			ContentCount++;
		}
		else if ( ImportAcctnum == 0 && isdigit ( xbuffer[0] ) != 0 )
		{
			ImportAcctnum = nsAtoi ( xbuffer );

			if ( LoadOneAccount ( ImportAcctnum ) != 1 )
			{
				sprintf ( StatementOne, "account not found for account number %d", ImportAcctnum );
				SaveError ( StatementOne );
				return ( -3 );
			}

			memcpy ( &MyAccount, &xaccount, sizeof(XACCOUNT) );

			if ( LoadOneImport ( ImportAcctnum ) != 1 )
			{
				sprintf ( StatementOne, "import control record not found for account number %d", ImportAcctnum );
				SaveError ( StatementOne );
				return ( -4 );
			}

			memcpy ( &MyImport, &ximport, sizeof(XIMPORT) );
#ifdef USE_PARSE_FUNCTIONS
			if ( MyImport.xautoparse[0] != 'Y' )
			{
				for ( xp = 0; xp < ParseCount; xp++ )
				{
					if ( nsStrcmp ( ParseArray[xp].Name, MyImport.xparsefunc ) == 0 )
					{
						Parse = ParseArray[xp].Function;
						break;
					}
				}

				if ( xp >= ParseCount )
				{
					sprintf ( StatementOne, 
						"Function %s not found for account number %d", 
								MyImport.xparsefunc, ImportAcctnum );
					SaveError ( StatementOne );
					return ( -5 );
				}
			}
			else if ( Verbose == 1 )
			{
				printf ( "ref %d date %d payee %d inc %d dec %d<br>\n",
					MyImport.xrefnum, MyImport.xdate, MyImport.xpayee, MyImport.xincrease, MyImport.xdecrease );
			}
#endif
		}
		else if ( AutoStatus == -1 && isdigit ( xbuffer[0] ) != 0 )
		{
			AutoStatus = nsAtoi ( xbuffer );
		}
		else if ( Verbose == -1 && isdigit ( xbuffer[0] ) != 0 )
		{
			Verbose = nsAtoi ( xbuffer );
		}
		else if ( ReportSkips == -1 && isdigit ( xbuffer[0] ) != 0 )
		{
			ReportSkips = nsAtoi ( xbuffer );
		}

		if ( ContentCount == CONTENT_BEFORE_FILE )
		{
			break;
		}
	}

	if ( ContentCount != CONTENT_BEFORE_FILE )
	{
		sprintf ( xbuffer, "File does not contain %d Content- statements", CONTENT_BEFORE_FILE );
		SaveError ( xbuffer );
		return ( -6 );
	}
	
	/*--------------------------------------------------------------
		read actual data.
	--------------------------------------------------------------*/

	LoadSystem ();
	FirstTrx = LastTrx = xsystem.xtrxnext;
	ImportCount = AcctErrorCount = 0;
	lineno = 0;

	printf ( "<pre>\n" );

	if ( Verbose )
	{
		printf ( "Identifying data in %s\n", tempfn );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		TrimLeft ( xbuffer );

		LineLength = nsStrlen ( xbuffer );
		
		if ( LineLength == 0 )
		{
			continue;
		}

		if ( nsStrncmp ( xbuffer, "--------", 8 ) == 0 )
		{
			break;
		}

		if ( BufferCount + LineLength > BufferSize )
		{
			BufferSize = BufferSize + SMALLBUFSZ * 10;
			if (( BigBuffer = realloc ( BigBuffer, BufferSize )) == NULL )
			{
				fprintf ( stderr, "Cannot realloc BigBuffer, %s\n", strerror(errno) );
				return ( -7 );
			}

			if ( Verbose )
			{
				printf ( "realloc %d\n", BufferSize );
			}
		}

		memcpy ( &BigBuffer[BufferCount], xbuffer, LineLength );
		BufferCount += LineLength;

		if ( Verbose )
		{
			webEntities((unsigned char *)xbuffer,WE_LOW,(unsigned char **)&zbuffer);
			printf ( "%s", zbuffer );
		}

	}

	nsFclose ( fp );

	ptrFileType = BufferType ( (unsigned char *)BigBuffer, BufferCount, Verbose );
	if ( Verbose )
	{
		printf ( "Data type %s\n", ptrFileType->Description );
	}

	switch ( ptrFileType->TypeID )
	{
		case SHS_FILE_TYPE_FINANCE_CSV:
			sprintf ( ScratchFile, "%s/scratch_%d.csv", TEMPDIR, getpid() );
			break;
		case SHS_FILE_TYPE_FINANCE_QIF:
			sprintf ( ScratchFile, "%s/scratch_%d.qif", TEMPDIR, getpid() );
			break;
		case SHS_FILE_TYPE_FINANCE_OFX:
			sprintf ( ScratchFile, "%s/scratch_%d.ofx", TEMPDIR, getpid() );
			break;
		case SHS_FILE_TYPE_UNKNOWN:
		default:
			printf ( "Cannot import file type %s\n", ptrFileType->Description );
			return ( -8 );
	}

	if (( fpScratch = fopen ( ScratchFile, "w" )) == (FILE *) 0 )
	{
		printf ( "Cannot create scratch file %s\n", ScratchFile );
		return ( -9 );
	}

	if ( fwrite ( BigBuffer, 1, BufferCount, fpScratch ) != BufferCount )
	{
		printf ( "Write error on scratch file, %s\n", strerror(errno) );
		return ( -10 );
	}

	nsFclose ( fpScratch );

	switch ( ptrFileType->TypeID )
	{
		case SHS_FILE_TYPE_FINANCE_CSV:
			ImportCSV ( ScratchFile );
			break;
		case SHS_FILE_TYPE_FINANCE_QIF:
			ImportQIF ( ScratchFile );
			break;
		case SHS_FILE_TYPE_FINANCE_OFX:
			ImportOFX ( ScratchFile );
			break;
	}

	if ( ImportCount > 0 )
	{
		/*--------------------------------------------------------------
			update previous import date 
		--------------------------------------------------------------*/
		sprintf ( StatementOne, 
				"update import set previous = '%04d-%02d-%02d' where acctnum = %d", 
						MaxDate.year4, MaxDate.month, MaxDate.day, ImportAcctnum );

		if ( Verbose )
		{
			printf ( "%s\n", StatementOne );
		}

		Affected = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );

		if ( Affected == 0 )
		{
			SaveError ( "update last transaction date failed" );
			AcctErrorCount++;

			if ( Verbose == 1 )
			{
				printf ( "FAILED: %s<br>\n", StatementOne );
			}
		}

		/*--------------------------------------------------------------
			fixit - this is NOT multi-user!
		--------------------------------------------------------------*/
		sprintf ( StatementOne, "update system set trxnext = %ld", xsystem.xtrxnext );
		Affected = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
		if ( Affected == 0 )
		{
			SaveError ( "update system next transaction number failed" );
			AcctErrorCount++;

			if ( Verbose == 1 )
			{
				printf ( "FAILED: %s<br>\n", StatementOne );
			}
		}
	}

	if ( Verbose == 0 && ReportCount > 0 )
	{
		printf ( "skipped %d transaction, %d previous %d closed<br>\n", DateCount, DatePrevious, DateClosed );
	}

#ifdef USE_POPUP
	sprintf ( StatementOne, "imported %d transactions, %d errors", ImportCount, AcctErrorCount );
	SaveError ( StatementOne );
#else
	printf ( "imported %d transactions, %d errors<br>\n", ImportCount, AcctErrorCount );
#endif

	if ( ImportCount > 0 )
	{
#ifdef USE_POPUP
		sprintf ( StatementOne, "transaction numbers %ld to %ld", FirstTrx, LastTrx );
		SaveError ( StatementOne );
#else
		printf ( "added transaction numbers %ld to %ld<br>\n", FirstTrx, LastTrx );
#endif
	}

	printf ( "</pre>\n" );

	return ( 0 );
}
