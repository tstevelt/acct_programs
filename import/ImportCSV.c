/*----------------------------------------------------------------------------
	Program : ImportCSV.c
	Author  : Tom Stevelt
	Date    : 12/11/2020
	Synopsis: Import CSV file into accounting system. May not work for 
			  liability and equity accounts if the file has two columns
			  for debits and credits instead of one amount column.
	Return  : 
	Referenc: https://www.fool.com/the-blueprint/debit-vs-credit/

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		12/31/2020	Deal with Germam American amounts ($123.45)

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

#define		SMALLBUFSZ	1024

/*---------------------------------------------------------------------------
Trying to eliminate work station pre-processors
FixCapitalOne.sh - fixes lines, adds 0.0 for missing amount
FixFidelity.sh - fixes lines, removes spaces, substitues words
FixPNC.sh - fixes lines, removes spaces. Still needs custom ParsePNC in import 
PreProcTDA - this one combines OPTION legs, not planning to eliminate it
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
PNC        : Date,Description,Withdrawals,Deposits,Balance
Capital One: Stage, Transaction Date, Posted Date, Card No., Description, Category, Debit, Credit
Chase      : Transaction Date,Post Date,Description,Category,Type,Amount
TD Ameritra: DATE,TRANSACTION ID,DESCRIPTION,QUANTITY,SYMBOL,PRICE,COMMISSION,AMOUNT,REG FEE,SHORT-TERM RDM FEE,FUND REDEMPTION FEE, DEFERRED SALES CHARGE
Fidelity   : Run Date,Action,Symbol,Security Description,Security Type,Quantity,Price ($),Commission ($),Fees ($),Accrued Interest ($),Amount ($),Settlement Date
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
Trying to automatically figure out column numbers and remove from import table.
Assume if there are two amount fields that we reverse sign on withdrawals and credits
---------------------------------------------------------------------------*/

static	int		DateColumn = -1;
static	int		PayeeColumn = -1;
static	int		RefnumColumn = -1;
static	int		AmountColumn = -1;
static	int		DecreaseColumn = -1;
static	int		IncreaseColumn = -1;

static	int		DebugImportCSV = 0;

static int GetColumnNumbers ()
{
	int		ndx;
	int		NumberFound = 0;

	for ( ndx = 0; ndx < tokcnt; ndx++ )
	{
		if ( DateColumn == -1 &&nsStrcasestr ( tokens[ndx], "date" ) != (char *)0 )
		{
			DateColumn = ndx;
			NumberFound++;
		}
		else if (( PayeeColumn == -1 && nsStrcasestr ( tokens[ndx], "merchant name" ) != (char *)0 ) ||
				 ( PayeeColumn == -1 && nsStrcasestr ( tokens[ndx], "payee" ) != (char *)0 ) ||
				 ( PayeeColumn == -1 && nsStrcasestr ( tokens[ndx], "description" ) != (char *)0 ))
		{
			PayeeColumn = ndx;
			NumberFound++;
		}
		else if ( AmountColumn == -1 && nsStrcasestr ( tokens[ndx], "amount" ) != (char *)0 )
		{
			AmountColumn = ndx;
			NumberFound++;
		}
		else if (( nsStrcasestr ( tokens[ndx], "withdrawals" ) != (char *)0 ) ||
				 ( nsStrcasestr ( tokens[ndx], "credit" ) != (char *)0 ))
		{
			DecreaseColumn = ndx;
			NumberFound++;
		}
		else if (( nsStrcasestr ( tokens[ndx], "deposits" ) != (char *)0 ) ||
				 ( nsStrcasestr ( tokens[ndx], "debit" ) != (char *)0 ))
		{
			IncreaseColumn = ndx;
			NumberFound++;
		}
		else if (( nsStrcasestr ( tokens[ndx], "category" ) != (char *)0 ) ||
				 ( nsStrcasestr ( tokens[ndx], "refnum" ) != (char *)0 ) ||
				 ( nsStrcasestr ( tokens[ndx], "symbol" ) != (char *)0 ) ||
				 ( nsStrcasestr ( tokens[ndx], "type" ) != (char *)0 ))
		{
			RefnumColumn = ndx;
			NumberFound++;
		}
	}
	
	if ( RefnumColumn == -1 )
	{
		RefnumColumn = PayeeColumn;
	}

	if ( DebugImportCSV )
	{
		printf ( "DateColumn     %d\n", DateColumn );
		printf ( "PayeeColumn    %d\n", PayeeColumn );
		printf ( "RefnumColumn   %d\n", RefnumColumn );
		printf ( "AmountColumn   %d\n", AmountColumn );
		printf ( "DecreaseColumn %d\n", DecreaseColumn );
		printf ( "IncreaseColumn %d\n", IncreaseColumn );
		printf ( "NumberFound    %d\n", NumberFound );
	}

	return ( NumberFound );
}


static long ParseAmount ( char *Token, int lineno )
{
	char	AmountBuffer[128];
	int		BufNdx, AmtNdx, AmtLen, SawParen, SawDecimal, SawMinus, DecimalCount;
	long	CalculatedAmount;

	AmtLen = nsStrlen ( Token );
	SawParen = 0;
	SawDecimal = 0;
	SawMinus = 0;
	DecimalCount = 0;
	
	for ( AmtNdx = 0, BufNdx = 0; AmtNdx < AmtLen; AmtNdx++ )
	{
		if ( Token[AmtNdx] == '$' )
		{
			continue;
		}
		else if ( Token[AmtNdx] == '(' )
		{
			SawParen = 1;
			continue;
		}
		else if ( Token[AmtNdx] == ')' )
		{
			continue;
		}
		else if ( Token[AmtNdx] == ',' )
		{
			continue;
		}
		else if (( Token[AmtNdx] < ' ' ) || ( Token[AmtNdx] > 127 ))
		{
			continue;
		}
		else if ( Token[AmtNdx] == '.' )
		{
			SawDecimal = 1;
		}
		else if ( Token[AmtNdx] == '-' )
		{
			SawMinus = 1;
		}
		else if (( Token[AmtNdx] >= '0' ) &&
				 ( Token[AmtNdx] <= '9' ))
		{
			if ( DecimalCount < 2 )
			{
				AmountBuffer[BufNdx++] = Token[AmtNdx];
				AmountBuffer[BufNdx  ] = '\0';
			}
			if ( SawDecimal )
			{
				DecimalCount++;
			}
		}
		else if ( Verbose )
		{
			printf ( "Unknown char [%c] in amount field<br>\n", Token[AmtNdx] );
		}
	}

	switch ( DecimalCount )
	{
		case 0: nsStrcat ( AmountBuffer, "00" ); break;
		case 1: nsStrcat ( AmountBuffer, "0" ); break;
		case 2: break;
		default: 
			if ( Verbose ) 
			{ 
				printf ( "Wrong decimal count %d in line %d<br>\n", DecimalCount, lineno ); 
			}
			break;
	}

	CalculatedAmount = nsAtol ( AmountBuffer );

	if ( SawMinus || SawParen )
	{
		CalculatedAmount = 0L - CalculatedAmount;
	}

	return ( CalculatedAmount );
}

int ImportCSV ( char *Filename )
{
	FILE	*fp;
	char	xbuffer[SMALLBUFSZ];
	int		lineno = 0;
	int		rv;
	long	AmountAmount, DecreaseAmount, IncreaseAmount;

	if ( Verbose )
	{
		printf ( "Starting ImportCSV<br>\n" );
	}

	if (( fp = fopen ( Filename, "r" )) == (FILE *)0 )
	{
		return ( -1 );
	}

	while ( fgets ( xbuffer, sizeof(xbuffer), fp ) != (char *)0 )
	{
		lineno++;

		if (( tokcnt = GetTokensCSV ( xbuffer, tokens, MAXTOKS, 0, 0 )) < 3 )
		{
			printf ( "tokcnt %d on line %d\n", tokcnt, lineno );
			continue;
		}

		if ( lineno == 1 )
		{
			if (( rv = GetColumnNumbers ()) < 0 )
			{
				return ( -2 );
			}

			if ( Verbose )
			{
				printf ( "Found %d columns\n", rv );
			}
			continue;
		}

		AmountAmount = DecreaseAmount = IncreaseAmount = 0L;

		if ( AmountColumn != -1 )
		{
			AmountAmount = ParseAmount ( tokens[AmountColumn], lineno );

			NewTrxd.xamount = AmountAmount;
		}
		else
		{
			if ( DecreaseColumn != -1 )
			{
				DecreaseAmount = ParseAmount ( tokens[DecreaseColumn], lineno );

				if ( DecreaseAmount > 0 )
				{
					DecreaseAmount = 0 - DecreaseAmount;
				}
			}

			if ( IncreaseColumn != -1 )
			{
				IncreaseAmount = ParseAmount ( tokens[IncreaseColumn], lineno );
			}

			if ( DebugImportCSV )
			{
				printf ( "Decrease %ld Increase %ld\n", DecreaseAmount, IncreaseAmount );
			}

			if ( DecreaseAmount != 0 )
			{
				NewTrxd.xamount = DecreaseAmount;
			}
			else if ( IncreaseAmount != 0 )
			{
				NewTrxd.xamount = IncreaseAmount;
			}
			else
			{
				// continue;  -- SaveTransaction will report it and return
				NewTrxd.xamount = 0;
			}
		}

		StrToDatevalFmt ( tokens[DateColumn], DATEFMT_MM_DD_YYYY, &NewTrxh.xtrxdate );
		snprintf ( NewTrxd.xpayee, sizeof(NewTrxd.xpayee), "%s", tokens[PayeeColumn] );
		snprintf ( NewTrxh.xrefnum, sizeof(NewTrxh.xrefnum), "%s", tokens[RefnumColumn] );
		if ( Verbose )
		{
			printf ( "Save %04d-%02d-%02d [%s] [%s] %ld\n",
				NewTrxh.xtrxdate.year4, NewTrxh.xtrxdate.month, NewTrxh.xtrxdate.day,
				NewTrxh.xrefnum, NewTrxd.xpayee, NewTrxd.xamount );
		}
		SaveTransaction ();
	}

	nsFclose ( fp );

	if ( Verbose )
	{
		printf ( "ImportCSV: processed %d lines\n", lineno );
	}

	return ( 0 );
}
