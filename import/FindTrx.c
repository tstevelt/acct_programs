/*--------------------------------------------------------------
	Program : FindTrx.c

	Purpose : Find previously imported transaction which is the
			  counter part to this one.  For instance, after 
			  importing checks there is one transaction record for 
			  a check to nat city CC for 650.17.  If we are now
			  importing nat city CC, we will get one line for 
			  a payment of 650.17.  we want to find the original
			  check and enter this new line with that transaction 
			  number, instead of winding up with two 
			  out-of-balance transactions.

	Return  : Zero if we could not find transaction.
			  Transaction number if we found a 'candidate'.

	Return  : zero - not found,
			  one  - did find.

--------------------------------------------------------------*/
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

int FindTrx ( long *TrxNumber, short *SeqNumber )
{
	int			rv = 0;
	DATEVAL		EarlierDate, LaterDate;
	DBY_QUERY	*Query;
	long		TestAmount;
	
	*TrxNumber = 0L;
	*SeqNumber = 0;

	DateAdd ( &NewTrxh.xtrxdate, -7, &EarlierDate );
	DateAdd ( &NewTrxh.xtrxdate,  7, &LaterDate );

	TestAmount = NewTrxd.xamount;

	sprintf ( StatementOne,
		"select trxh.trxnum from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum != %d and seqnum = 1 and trxdate >= '%04d-%02d-%02d' and trxdate <= '%04d-%02d-%02d' and status <= %d and (amount = %ld or amount = %ld)",
			ImportAcctnum,
			EarlierDate.year4,
			EarlierDate.month,
			EarlierDate.day,
			LaterDate.year4,
			LaterDate.month,
			LaterDate.day,
			STATUS_BALANCE,
			TestAmount,
			0L - TestAmount );

	Query = dbySelect ( "import", &MySql, StatementOne, LOGFILENAME );

	if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		*TrxNumber = safe_atol ( Query->EachRow[0] );

		sprintf ( StatementOne,
			"select max(seqnum) from trxd where trxnum = %ld", (long) *TrxNumber  );

		Query = dbySelect ( "import", &MySql, StatementOne, LOGFILENAME );

		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			*SeqNumber = (short) ( 1 +  safe_atoi ( Query->EachRow[0] ) );

			rv = 1;
		}

		if ( Verbose )
		{
			printf ( "FindTrx: found one %ld %d<br>\n", *TrxNumber, *SeqNumber );
		}
	}

	return ( rv );
}
