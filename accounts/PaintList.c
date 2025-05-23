/*----------------------------------------------------------------------------
	Program : PaintList.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: List selected accounts
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


#include	"accounts.h"

#define		SHOWCOUNT(x)	x == 'C' || x == 'B'
#define		SHOWAMT(x)		x == 'A' || x == 'B'
#define		SHOWDATE(x)		x == 'A' || x == 'C' || x == 'B'

void PaintList ()
{
	DBY_QUERY		*QueryOne;
	DBY_QUERY		*QueryTwo;
	int				MyCount;
	int				TotalColumns;
	int				Cracked;
	int				TrxCount;
	DATEVAL			LatestTrxdate;
	int				HasImport;
	char			CurType;
	int				ThisVal, SubTotal, GrandTotal;

	switch ( xsystem.xshowamts[0]) 
	{
		case 'N':
			TotalColumns = 2;
			break;
		case 'A':
		case 'C':
			TotalColumns = 4;
			break;
		case 'B':
			TotalColumns = 5;
			break;
	}
	sprintf ( StatementOne, "select %s from account ", FIELDS_ACCOUNT );
	if ( AC_Mode == 'A' )
	{
		strcat ( StatementOne, "where accttype = 'A' or accttype = 'E' or accttype = 'L' " );
	}
	else
	{
		strcat ( StatementOne, "where accttype = 'I' or accttype = 'X' " );
	}
	strcat ( StatementOne, "order by accttype, acctnum" );
	// strcat ( StatementOne, "order by accttype, acctname" );

	QueryOne = dbySelect ( "accounts", &MySql, StatementOne, LOGFILENAME );

	SubTotal = GrandTotal = 0;
	MyCount = 0;
	CurType = '?';
	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		MyCount++;

		xaccount.xacctnum = safe_atol ( QueryOne->EachRow[ACCOUNT_acctnum] );
		xaccount.xaccttype[0] = QueryOne->EachRow[ACCOUNT_accttype][0];
		snprintf ( xaccount.xacctname, sizeof(xaccount.xacctname), "%s", QueryOne->EachRow[ACCOUNT_acctname] );
		xaccount.xopenbal = safe_atol ( QueryOne->EachRow[ACCOUNT_openbal] );
		xaccount.xcurrbal = safe_atol ( QueryOne->EachRow[ACCOUNT_currbal] );

		/*--------------------------------------------------------------
			get count of transactions which are pre-entered, new or oob.
		--------------------------------------------------------------*/
		sprintf ( StatementTwo, "select count(*) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %ld and status < %d",
						xaccount.xacctnum, STATUS_OPEN );
		QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );
		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
		{
			Cracked = safe_atoi ( QueryTwo->EachRow[0] );
		}
		else
		{
			Cracked = 0;
		}

		if ( (SHOWCOUNT(xsystem.xshowamts[0])) || (SHOWDATE(xsystem.xshowamts[0]) ))
		{
			/*--------------------------------------------------------------
				get count of transactions for this acct
			// sprintf ( StatementTwo, "select count(*) from trxd where acctnum = %ld", xaccount.xacctnum );
			--------------------------------------------------------------*/
			sprintf ( StatementTwo, 
				"select count(*), max(trxdate) from trxd, trxh where trxh.trxnum = trxd.trxnum and trxd.acctnum = %ld", 
					xaccount.xacctnum );
			QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );
			if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				TrxCount = safe_atoi ( QueryTwo->EachRow[0] );
				StrToDatevalFmt ( QueryTwo->EachRow[1], DATEFMT_YYYY_MM_DD, &LatestTrxdate );
			}
			else
			{
				TrxCount = 0;
				LatestTrxdate.month = 0;
			}

			/*----------------------------------------------------------
				see if this account has an import routine.
			----------------------------------------------------------*/
			sprintf ( StatementTwo, "select count(*) from import where acctnum = %ld", xaccount.xacctnum );
			QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );
			if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				HasImport = safe_atoi ( QueryTwo->EachRow[0] );
			}
			else
			{
				HasImport = 0;
			}
		}

		if ( MyCount == 1 )
		{
			printf ( "<table class='AppWide'>\n" );
			printf ( "<tr>\n" );
			printf ( "<th align='left'>number</th>\n" );
			printf ( "<th align='left'>name</th>\n" );
			if ( SHOWCOUNT(xsystem.xshowamts[0]) )
			{
				printf ( "<th align='left'>count</th>\n" );
			}
			if ( SHOWDATE(xsystem.xshowamts[0]) )
			{
				printf ( "<th align='left'>latest import</th>\n" );
			}
			if ( SHOWAMT(xsystem.xshowamts[0]) )
			{
				printf ( "<th align='right'>current</th>\n" );
			}
			printf ( "</tr>\n" );
		}

		if ( CurType != xaccount.xaccttype[0] )
		{
			if ( SHOWAMT(xsystem.xshowamts[0]) )
			{
				switch ( CurType )
				{
					case 'I': 
					case 'A': 
						printf ( "<tr>\n" );
						printf ( "<td align='right' colspan='%d'>sub-total</td>\n" ,
									xsystem.xshowamts[0] == 'B' ? 4 : 3 );
						printf ( "<td align='right'>\n" );
						printf ( "%.2f", (double) SubTotal / 100.0 );
						printf ( "</td>\n" );
						printf ( "</tr>\n" );
						GrandTotal = SubTotal;
						SubTotal = 0.0;
						break;

					case 'E': 
						SubTotal = 0.0;
						break;
				}
			}
			
			CurType = xaccount.xaccttype[0];

			printf ( "<tr>\n" );
			printf ( "<th align='left' colspan='%d'>\n", TotalColumns );
			switch ( CurType )
			{
				case 'A': 
					printf ( "asset accounts" ); 
					break;

				case 'E':
					printf ( "equity accounts" );
					break;

				case 'L': 
					printf ( "liability accounts" );
					break;

				case 'I': 
					printf ( "income categories" );
					break;

				case 'X': 
					printf ( "expense categories" );
					break;

			}
			printf ( "</th>\n" );
			printf ( "</tr>\n" );
		}

		printf ( "<tr>\n" );

		printf ( "<td><a href='accounts.cgi?ac=%c&register=%ld'>%ld</a></td>\n", 
					AC_Mode, xaccount.xacctnum, xaccount.xacctnum );

		printf ( "<td><a href='accounts.cgi?ac=%c&edit=%ld'>%s</a></td>\n", 
					AC_Mode, xaccount.xacctnum, xaccount.xacctname );

		if ( SHOWCOUNT(xsystem.xshowamts[0]) )
		{
			printf ( "<td>%d</td>\n", TrxCount );
		}
		if ( SHOWDATE(xsystem.xshowamts[0]) )
		{
			//printf ( "<td>%02d/%02d/%02d</td>\n", 4, 1, 99 );
			if ( HasImport && LatestTrxdate.month )
			{
				printf ( "<td>%02d/%02d/%02d</td>\n", LatestTrxdate.month, LatestTrxdate.day, LatestTrxdate.year2 );
			}
			else
			{
				printf ( "<td>&nbsp;</td>\n" );
			}
		}
		if ( SHOWAMT(xsystem.xshowamts[0]) )
		{
			ThisVal = xaccount.xcurrbal;
			SubTotal += ThisVal;
			printf ( "<td align='right'" );
			if ( Cracked > 0 )
			{
				printf ( "bgcolor='%s'", COLOR_SOMETHING );
			}
			printf ( ">%.2f</td>\n",  (double) ThisVal / 100.0 );
		}

		printf ( "</tr>\n" );
	}

	if ( MyCount == 0 )
	{
		printf ( "<table class='AppWide'>\n" );
	}
	else if ( SHOWAMT(xsystem.xshowamts[0]) )
	{
		printf ( "<tr>\n" );
		printf ( "<td align='right' colspan='%d'>sub-total</td>\n",
						xsystem.xshowamts[0] == 'B' ? 4 : 3 );
		printf ( "<td align='right'>\n" );
		printf ( "%.2f", (double ) SubTotal / 100.0 );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

		if ( AC_Mode == 'C' )
		{
			GrandTotal -= SubTotal;

			if ( GrandTotal != 0.0 )
			{
				printf ( "<tr>\n" );
				printf ( "<td align='right' colspan='%d'>%s</td>\n",
						xsystem.xshowamts[0] == 'B' ? 4 : 3 ,
						AC_Mode == 'A' ? "out of balance" : "earnings"  );

				printf ( "<td align='right'>\n" );
				printf ( "%.2f", (double) GrandTotal / 100.0 );
				printf ( "</td>\n" );
				printf ( "</tr>\n" );
			}
		}
	}

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='%d'>\n", TotalColumns );

	printf ( "<input type='button' name='add' value='add' " );
	if ( xmember.xmrole[0] == ROLE_DEMO )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:window.open(\"accounts.cgi?ac=%c&add=account\",\"_self\");'>\n", AC_Mode );
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>" );

}
