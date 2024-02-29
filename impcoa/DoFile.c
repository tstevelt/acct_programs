/*----------------------------------------------------------------------------
	Program : DoFile.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Read the uploaded file and insert account records.
	Return  : 
----------------------------------------------------------------------------------*/
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

#include	"impcoa.h"

int DoFile ( char *tempfn )
{
	DBY_QUERY		*Query;
	FILE	*fp;
	char	xbuffer[1024];
	int		ContentCount;
	int		lineno;
	int		Affected, ImportCount, AcctErrorCount;
	int		AutoNumber;
	int		xt;
	char	*fldname;

	/*--------------------------------------------------------------
		open the file
	--------------------------------------------------------------*/
	if (( fp = fopen ( tempfn, "r" )) == (FILE *)0 )
	{
		sprintf ( StatementOne, "Can not open file %s, errno %d", tempfn, errno );
		SaveError ( StatementOne );
		return ( -1 );
	}

	/*--------------------------------------------------------------
		read first part of file to find account number and 
		parse function.  break and go in to second while loop
		to read the actual data.
	--------------------------------------------------------------*/
	lineno = 0;
	ContentCount = 0;
	ImportCount = AcctErrorCount = 0;
	lineno = 0;
	AutoNumber = -1;

	printf ( "<pre>\n" );

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		TrimRight ( xbuffer );

		if ( nsStrlen ( xbuffer ) == 0 )
		{
			continue;
		}

		if ( nsStrncmp ( xbuffer, "Content-", 8 ) == 0 )
		{
			ContentCount++;
			continue;
		}

		if ( nsStrncmp ( xbuffer, "--------", 8 ) == 0 )
		{
			if ( ContentCount == 3 )
			{
				break;
			}
			else
			{
				continue;
			}
		}

// printf ( "%s\n", xbuffer );

		if ( ContentCount == 1 )
		{
			switch ( xbuffer[0] )
			{
				case 'A': AutoNumber = 1; break;
				case 'M': AutoNumber = 0; break;
				default : 
					printf ( "invalid value for autonum.  program canceled.\n" );
					return ( -2 );
			}
		}
		else if ( ContentCount == 3 && AutoNumber != -1 )
		{
			tokcnt = GetTokensCSV ( xbuffer, tokens, MAXTOKS, 0, 0 );

			if ( AutoNumber == 0 && tokcnt < 3 )
			{
				continue;
			}
			else if ( AutoNumber == 1 && tokcnt < 2 )
			{
				continue;
			}

			/*--------------------------------------------------------------
				get two or three tokens, depending on auto numbering.
			--------------------------------------------------------------*/
			xt = 0;
			if ( AutoNumber == 0 )
			{
				xaccount.xacctnum = safe_atol ( tokens[xt] );
				xt++;
			}

			xaccount.xaccttype[0] = toupper ( tokens[xt][0] );
			xt++;

			lset ( xaccount.xacctname, tokens[xt], sizeof(xaccount.xacctname) );
			xt++;

			/*--------------------------------------------------------------
				may get two more tokens, for tax form and line.
			--------------------------------------------------------------*/
			xaccount.xextranum[0] = '\0';
			xaccount.xextraline[0] = '\0';
			if ( xt < tokcnt )
			{
				lset ( xaccount.xextranum, tokens[xt], sizeof(xaccount.xextranum) );
				xt++;

				if ( xt < tokcnt )
				{
					lset ( xaccount.xextraline, tokens[xt], sizeof(xaccount.xextraline) );
					xt++;
				}
			}

			/*--------------------------------------------------------------
				make sure we got a valid account type.
			--------------------------------------------------------------*/
			switch ( xaccount.xaccttype[0] )
			{
				case 'A':
				case 'L':
				case 'E':
				case 'I':
				case 'X':
					if ( AutoNumber == 0 )
					{
						printf ( "%c %ld %.30s\n", 
							xaccount.xaccttype[0], xaccount.xacctnum, xaccount.xacctname );
					}
					else
					{
						printf ( "%c %.30s", 
							xaccount.xaccttype[0], xaccount.xacctname );
					}
					break;

				default :
					printf ( "ERROR: Unknown account type %c %ld %s\n",
						xaccount.xaccttype[0], xaccount.xacctnum, xaccount.xacctname );

					AcctErrorCount++;
					continue;
			}

			/*--------------------------------------------------------------
				if automatic numbering, get from and update system record.
			--------------------------------------------------------------*/
			if ( AutoNumber == 1 )
			{
				switch ( xaccount.xaccttype[0] )
				{
					case 'A': fldname = "assetnext"; break;
					case 'L': fldname = "liabnext"; break;
					case 'E': fldname = "equitynext"; break;
					case 'I': fldname = "incomenext"; break;
					case 'X': fldname = "expensenext"; break;
				}

				snprintf ( StatementTwo, sizeof(StatementTwo),
						"select %s from system", fldname );

				Query = dbySelect  ( "impcoa", &MySql, StatementTwo, LOGFILENAME );

				if (( Query->EachRow = mysql_fetch_row ( Query->Result )) == NULL )
				{
					printf ( "can not get next account number.  program canceled.\n" );
					return ( -3 );
				}

				xaccount.xacctnum = safe_atol ( Query->EachRow[0] );

				printf ( "%ld\n", xaccount.xacctnum );

				snprintf ( StatementTwo, sizeof(StatementTwo),
						"update system set %s = %ld where %s = %ld",
							fldname, xaccount.xacctnum + 1, fldname, xaccount.xacctnum );

				dbyUpdate  ( "impcoa", &MySql, StatementTwo, 0, LOGFILENAME );
			}

			switch ( xaccount.xaccttype[0] )
			{
				case 'I':
				case 'X':
					if ( xaccount.xextraline[0] == '\0' )
					{
						snprintf ( StatementOne, sizeof(StatementOne),
							"insert into account ( acctnum, accttype, acctname )  values ( %ld, '%c', '%s' )",
										xaccount.xacctnum, xaccount.xaccttype[0], xaccount.xacctname );
					}
					else
					{
						snprintf ( StatementOne, sizeof(StatementOne),
"insert into account ( acctnum, accttype, acctname, extranum, extraline )  values ( %ld, '%c', '%s', '%s', '%s' )",
			xaccount.xacctnum, xaccount.xaccttype[0], xaccount.xacctname, xaccount.xextranum, xaccount.xextraline );
					}
					break;

				default:
					snprintf ( StatementOne, sizeof(StatementOne),
						"insert into account ( acctnum, accttype, acctname )  values ( %ld, '%c', '%s' )",
									xaccount.xacctnum, xaccount.xaccttype[0], xaccount.xacctname );
					break;
			}


			Affected = dbyInsert ( "impcoa", &MySql, StatementOne, 0, LOGFILENAME );

			if ( Affected == 1 )
			{
				ImportCount++;
			}
			else
			{
				printf ( "  ERROR: %ld NOT IMPORTED\n", xaccount.xacctnum );
				AcctErrorCount++;
			}
		}
	}

	if ( ContentCount != 3 )
	{
		SaveError ( "File does not contain 3 Content- statements" );
	}

	if ( ImportCount > 0 )
	{
	}
	
	sprintf ( StatementOne, "imported %d accounts, %d errors", ImportCount, AcctErrorCount );
	SaveError ( StatementOne );

	printf ( "</pre>\n" );

	return ( 0 );
}
