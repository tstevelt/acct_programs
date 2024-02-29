/*----------------------------------------------------------------------------
	Program : Lookup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint transaction list matching user's lookup criteria
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

#include	"entry.h"

static	DBY_QUERY     *MyQuery;

static int MyEachTrxd ()
{
	static int firstpass = 1;

	if ( firstpass )
	{
		firstpass = 0;
		printf ( "<table class='AppWide'>\n" );
		printf ( "<tr>\n" );

		printf ( "<td>trxnum</td>\n" );
		printf ( "<td>status</td>\n" );
		printf ( "<td>refnum</td>\n" );
		printf ( "<td>trxdate</td>\n" );
		printf ( "<td>payee</td>\n" );
		printf ( "<td>amount</td>\n" );
		if ( TrxArray[1].d.xacctnum == 0 )
		{
			printf ( "<td>account</td>\n" );
		}

		printf ( "</tr>\n" );
	}

	printf ( "<td>" );
#ifdef THIS_IS_FOR_POPUP
	printf ( "<a href='#' onclick='javascript:SetId(%ld);'>%ld</a>", nsAtol(MyQuery->EachRow[0]), nsAtol(MyQuery->EachRow[0]) );
#else
	printf ( "<a href='entry.cgi?what=find&trxnum=%ld'>%ld</a>", nsAtol(MyQuery->EachRow[0]), nsAtol(MyQuery->EachRow[0]) );
#endif
	printf ( "</td>\n" );

	printf ( "<td>%s</td>\n",  StatusDescr(nsAtoi(MyQuery->EachRow[1]),2) );
	printf ( "<td>%s</td>\n",  MyQuery->EachRow[2] );
	printf ( "<td>%s</td>\n",  MyQuery->EachRow[3] );
	printf ( "<td>%s</td>\n",  MyQuery->EachRow[4] );
	printf ( "<td>%.2f</td>\n",  nsAtof ( MyQuery->EachRow[5] ) / 100.0 );

	if ( TrxArray[1].d.xacctnum == 0 )
	{
		printf ( "<td>%s</td>\n",  MyQuery->EachRow[6] );
	}

	printf ( "</tr>\n" );

	return ( 0 );
}

int Lookup ()
{
	char	Fields[128];
	char	Tables[128];
	char	Fragment[128];
	DATEVAL	sdt, edt;
	int		rv;
	int		UserCriteriaCount = 0;

	snprintf ( WhereClause, sizeof(WhereClause), "trxh.trxnum = trxd.trxnum and trxd.seqnum <= 2" );

	if ( TrxArray[1].d.xacctnum > 0 )
	{
		sprintf ( Fields, "trxh.trxnum, status, refnum, trxdate, payee, amount" );
		sprintf ( Tables, "trxh, trxd" );

		sprintf ( Fragment, " and trxd.acctnum = %ld", TrxArray[1].d.xacctnum );

		nsStrcat ( WhereClause, Fragment );

		if ( xtrxh.xtrxdate.month == 0 )
		{
			LoadOneAccount ( TrxArray[1].d.xacctnum );
			if ( TimeFrameDates ( xaccount.xdaterange, &sdt, &edt ) == 0 )
			{
				sprintf ( Fragment, " and trxh.trxdate >= '%04d-%02d-%02d'", sdt.year4, sdt.month, sdt.day );

				nsStrcat ( WhereClause, Fragment );
			}
		}
	}
	else
	{
		sprintf ( Fields, "trxh.trxnum, status, refnum, trxdate, payee, amount, acctname" );
		sprintf ( Tables, "trxh, trxd, account" );
		sprintf ( Fragment, " and trxd.acctnum = account.acctnum" );
		nsStrcat ( WhereClause, Fragment );
	}

	/*----------------------------------------------------------
		top of screen
	----------------------------------------------------------*/
	if ( nsStrlen ( xtrxh.xrefnum ) > 0 )
	{
		if ( dbyWildCards ( xtrxh.xrefnum ) > 0 )
		{
			sprintf ( Fragment, " and trxh.refnum like '%s'", xtrxh.xrefnum );
		}
		else
		{
			sprintf ( Fragment, " and trxh.refnum = '%s'", xtrxh.xrefnum );
		}
		nsStrcat ( WhereClause, Fragment );
		UserCriteriaCount++;
	}

	if ( xtrxh.xtrxdate.month > 0 )
	{
		sprintf ( Fragment, " and trxh.trxdate = '%04d-%02d-%02d'", 
			xtrxh.xtrxdate.year4, xtrxh.xtrxdate.month, xtrxh.xtrxdate.day );

		nsStrcat ( WhereClause, Fragment );
		UserCriteriaCount++;
	}


	/*----------------------------------------------------------
		detail (multiple lines) of screen
	----------------------------------------------------------*/
	for ( int xd = 1; xd <= 2; xd++ )
	{
		if ( TrxArray[xd].d.xacctnum > 0 )
		{
			sprintf ( Fragment, " and trxd.acctnum = '%ld'",  TrxArray[xd].d.xacctnum);
			nsStrcat ( WhereClause, Fragment );
			UserCriteriaCount++;
		}

		if ( nsStrlen ( TrxArray[xd].d.xpayee ) > 0 )
		{
			if ( dbyWildCards ( TrxArray[xd].d.xpayee ) > 0 )
			{
				sprintf ( Fragment, " and trxd.payee like '%s'", TrxArray[xd].d.xpayee );
			}
			else
			{
				sprintf ( Fragment, " and trxd.payee = '%s'", TrxArray[xd].d.xpayee );
			}
			nsStrcat ( WhereClause, Fragment );
			UserCriteriaCount++;
		}

		if ( TrxArray[xd].d.xamount != 0 )
		{
			sprintf ( Fragment, " and trxd.amount = '%ld'",  TrxArray[xd].d.xamount);
			nsStrcat ( WhereClause, Fragment );
			UserCriteriaCount++;
		}
	}

	if ( UserCriteriaCount == 0 )
	{
		SaveError ( "Please enter something to search for." );
		SaveError ( "Wildcards (asterisk) are allowed." );
		return ( 0 );
	}

	snprintf ( StatementOne, sizeof(StatementOne), 
			"select %s from %s where %s order by trxdate desc", Fields, Tables, WhereClause );

// printf ( "<p>%s<br>\n", StatementOne );

	MyQuery = dbySelect ( "entry", &MySql, StatementOne, LOGFILENAME );

	if ( MyQuery == (DBY_QUERY *) 0 )
	{
		SaveError ( "entry select returned NULL." );
		return ( -1 );
	}
	else if ( MyQuery->NumRows == 0 )
	{
		SaveError ( "Nothing matches selection criteria" );
		dbyFreeQuery ( MyQuery );
		MyQuery = (DBY_QUERY *) 0;
		return ( 0 );
	}
	else 
	{
		while (( MyQuery->EachRow = mysql_fetch_row ( MyQuery->Result )) != NULL )
		{
			if ( MyEachTrxd () != 0 )
			{
				break;
			}
		}
	}

	rv = MyQuery->NumRows;

	dbyFreeQuery ( MyQuery );
	MyQuery = (DBY_QUERY *) 0;

	return ( rv );

}
