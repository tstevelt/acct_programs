/*--------------------------------------------------------------
	Program : FindRepeating.c

	Purpose : Find previously imported transaction for this 
			  same account number, same dollar value, and 
			  open, cleared or reconciled.
			  If found, dupe sequence number 2 .. n.

	Method2 : Find by exact match of payee and refnum.

	Return  : Zero if we could not find transaction.
			  One to indicate success.

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		12/02/2020	If found by method two, then put all money
						on sequence number 2. kinda ... sorta.
	tms		12/03/2020	Incorporate Method 3 using nsSimilar(payee). 
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

double      dblScore;

int FindRepeating ( int Method, XTRXH *NewTrxh, XTRXD *NewTrxd, long *ThatTrxNumber )
{
	DBY_QUERY		*Query;
	char			MyStatement[4096];
	short			xs;
	int				Found;

	*ThatTrxNumber = 0L;

	switch ( Method )
	{
		case 1:
				sprintf ( MyStatement,
"select trxh.trxnum from trxh, trxd where trxh.trxnum = trxd.trxnum and trxh.trxnum != %ld and seqnum = 1 and status >= %d and acctnum = %ld and amount = %ld order by trxdate desc",
						NewTrxh->xtrxnum,
						STATUS_OPEN, 
						NewTrxd->xacctnum, 
						NewTrxd->xamount );
				break;

		case 2:
				if ( nsStrlen ( NewTrxd->xpayee ) == 0 )
				{
					return ( 0 );
				}
				if ( nsStrcmp ( NewTrxd->xpayee, "payee" ) == 0 )
				{
					return ( 0 );
				}
				if ( nsStrlen ( NewTrxh->xrefnum ) == 0 )
				{
					return ( 0 );
				}

				sprintf ( MyStatement,
"select trxh.trxnum from trxh, trxd where trxh.trxnum = trxd.trxnum and trxh.trxnum != %ld and seqnum = 1 and status >= %d and acctnum = %ld and payee = '%s' and refnum = '%s' order by trxdate desc",
						NewTrxh->xtrxnum,
						STATUS_OPEN, 
						NewTrxd->xacctnum, 
						NewTrxd->xpayee,
						NewTrxh->xrefnum );
				break;

		case 3:
				if ( nsStrlen ( NewTrxd->xpayee ) == 0 )
				{
					return ( 0 );
				}
				if ( nsStrcmp ( NewTrxd->xpayee, "payee" ) == 0 )
				{
					return ( 0 );
				}
				if ( nsStrlen ( NewTrxh->xrefnum ) == 0 )
				{
					return ( 0 );
				}

				sprintf ( MyStatement,
"select trxh.trxnum, trxd.payee from trxh, trxd where trxh.trxnum = trxd.trxnum and trxh.trxnum != %ld and seqnum = 1 and status >= %d and acctnum = %ld and refnum = '%s' order by trxdate desc",
						NewTrxh->xtrxnum,
						STATUS_OPEN, 
						NewTrxd->xacctnum, 
						NewTrxh->xrefnum );
				break;

	}

	Query = dbySelect ( "import", &MySql, MyStatement, LOGFILENAME );

	switch ( Method )
	{
		case 1:
		case 2:
			if (( Query->EachRow = mysql_fetch_row ( Query->Result )) == NULL )
			{
				return ( 0 );
			}
			break;
		case 3:
			Found = 0;
			while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
			{
				if (( dblScore = nsSimilar ( Query->EachRow[1], NewTrxd->xpayee )) > 0.5 )
				{
					Found = 1;
					break;
				}
			}
			if ( Found == 0 )
			{
				return ( 0 );
			}
			break;
	}

	*ThatTrxNumber = safe_atol ( Query->EachRow[0] );
	
	for ( xs = 2; xs <= MAXSEQNO; xs++ )
	{
		if ( LoadOneTrxd ( *ThatTrxNumber, xs ) == 0 )
		{
			break;
		}

		xtrxd.xtrxnum = NewTrxh->xtrxnum;

		if (( Method == 2 && xs == 2 ) ||
			( Method == 3 && xs == 2 ))
		{
			int		ReverseSign = 0;

			if ( LoadOneAccount ( xtrxd.xacctnum ) == 1 )
			{
				/*----------------------------------------------------------
					import only works for ASSET and LIABILITY accounts.
				----------------------------------------------------------*/
				switch ( MyAccount.xaccttype[0] )
				{
					case 'A':
						switch ( xaccount.xaccttype[0] )
						{
							case 'A':
							case 'X':
								ReverseSign = 1;
								break;
						}
						break;
					case 'L':
						switch ( xaccount.xaccttype[0] )
						{
							case 'L':
							case 'E':
							case 'I':
								ReverseSign = 1;
								break;
						}
						break;
				}
			}

			if ( ReverseSign == 0 )
			{
				xtrxd.xamount = NewTrxd->xamount;
			}
			else
			{
				xtrxd.xamount = 0 - NewTrxd->xamount;
			}
		}

		if ( InsertOneTrxd ( &xtrxd ) == 0 )
		{
			SaveError ( "InsertOneTrxd failed." );
			break;
		}
	}

	return ( 1 );
}
