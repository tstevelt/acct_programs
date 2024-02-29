/*----------------------------------------------------------------------------
	Program : LoadOpen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load <= OPEN transactions in date range
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

#include	"reconcile.h"

static int compare ( TRXREC *a, TRXREC *b )
{
	int		rv;

	if ( a->Sign < b->Sign )
	{
		return ( -1 );
	}
	if ( a->Sign > b->Sign )
	{
		return ( 1 );
	}

	rv = CompareDateval ( &a->h.xtrxdate, &b->h.xtrxdate );
	if ( rv < 0 )
	{
		return ( -1 );
	}
	if ( rv > 0 )
	{
		return ( 1 );
	}

	rv = nsStrcmp ( a->h.xrefnum, b->h.xrefnum );
	if ( rv < 0 )
	{
		return ( -1 );
	}
	if ( rv > 0 )
	{
		return ( 1 );
	}

	return ( 0 );
}

void LoadOpen ()
{
	DBY_QUERY		*Query;
	char			DatePhrase[80];

	sprintf ( StatementOne, 
		"select %s from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %ld",
				FLDS_TRXV, xaccount.xacctnum );
//		and seqnum = 1", 

	if ( StartDate.month > 0 )
	{
		sprintf ( DatePhrase, " and trxdate >= '%04d-%02d-%02d'",
						StartDate.year4, StartDate.month, StartDate.day );

		strcat ( StatementOne, DatePhrase );
	}

	if ( EndDate.month > 0 )
	{
		sprintf ( DatePhrase, " and trxdate <= '%04d-%02d-%02d'",
						EndDate.year4, EndDate.month, EndDate.day );

		strcat ( StatementOne, DatePhrase );
	}

	if ( StartDate.month == 0 || EndDate.month == 0 )
	{
		int		xl;

		xl = nsStrlen ( StatementOne );
		sprintf ( &StatementOne[xl], " and status <= %d", STATUS_OPEN );
	}

	Query = dbySelect ( "reconcile", &MySql, StatementOne, LOGFILENAME );

	TrxCount = 0;
	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		if ( TrxCount >= MAXTRX )
		{
			sprintf ( StatementOne, "Exceeds MAXTRX." );
			SaveError ( StatementOne );
			break;
		}

		TrxArray[TrxCount].h.xtrxnum = safe_atol ( Query->EachRow[TRXV_trxnum] );
		TrxArray[TrxCount].d.xtrxnum = safe_atol ( Query->EachRow[TRXV_trxnum] );
		TrxArray[TrxCount].h.xstatus = safe_atoi ( Query->EachRow[TRXV_status] );
		snprintf ( TrxArray[TrxCount].h.xrefnum,  8, "%s", Query->EachRow[TRXV_refnum] );
		snprintf ( TrxArray[TrxCount].d.xpayee,  40, "%s", Query->EachRow[TRXV_payee] );
		TrxArray[TrxCount].d.xamount = safe_atol ( Query->EachRow[TRXV_amount] );
		StringToDateval ( Query->EachRow[TRXV_trxdate], &TrxArray[TrxCount].h.xtrxdate );

		if ( TrxArray[TrxCount].d.xamount < 0 )
		{
			TrxArray[TrxCount].Sign = -1;
		}
		else
		{
			TrxArray[TrxCount].Sign = 1;
		}

		TrxCount++;
	}

	if (TrxCount > 1 )
	{
		qsort ( TrxArray, TrxCount, sizeof(TRXREC), (int(*)()) compare );
	}
}
