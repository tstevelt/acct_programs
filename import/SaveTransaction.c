/*----------------------------------------------------------------------------
	Program : SaveTransaction.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Save one transaction to database, write info to popup.
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

extern	double		dblScore;

static int DebugSaveTransaction = 0;

/*----------------------------------------------------------
	Capital One stupid memo
!Type:CCard
D01/04/2021
T-15.08
C*
PNETFLIX.COM
M1027: NETFLIX.COM
^

----------------------------------------------------------*/
static void BetterRefnum ()
{
	char	*cp;
	int		ChangeIt;

	if ( DebugSaveTransaction )
	{
		printf ( "REFNUM %s<br>\nPAYEE %s<br>\n", OriginalRefnum, OriginalPayee );
	}

	/*----------------------------------------------------------
		if memo completely diffrent than payee, don't change
	----------------------------------------------------------*/
	ChangeIt = 0;
	if (( cp = nsStrstr ( OriginalRefnum, OriginalPayee )) != (char *)0 )
	{
		ChangeIt = 1;
	}
	else if (( cp = nsStrstr ( OriginalPayee, OriginalRefnum )) != (char *)0 )
	{
		ChangeIt = 1;
	}


	if ( ChangeIt == 0 )
	{
		if ( DebugSaveTransaction )
		{
			printf ( "nsStrstr NULL twice<br>\n" );
		}
		return;
	}

	if ( MyAccount.xaccttype[0] == 'L' )
	{
		if ( NewTrxd.xamount < 0L )
		{
			sprintf ( NewTrxh.xrefnum, "PAYMENT" );
		}
		else
		{
			sprintf ( NewTrxh.xrefnum, "CHARGE" );
		}
		if ( DebugSaveTransaction )
		{
			printf ( "%s<br>\n", NewTrxh.xrefnum );
		}
	}
	else if ( MyAccount.xaccttype[0] == 'A' )
	{
		if ( NewTrxd.xamount < 0L )
		{
			sprintf ( NewTrxh.xrefnum, "CHECK" );
		}
		else
		{
			sprintf ( NewTrxh.xrefnum, "DEPOSIT" );
		}
		if ( DebugSaveTransaction )
		{
			printf ( "%s<br>\n", NewTrxh.xrefnum );
		}
	}
	if ( DebugSaveTransaction )
	{
		printf ( "account type %c<br>\n", MyAccount.xaccttype[0] );
	}
}

static void PrintHeader ()
{
	printf ( "SKIPPING THESE TRANSACTIONS\n" );

	if ( xsystem.xclosedate.month > 0 )
	{
		printf ( "%04d-%02d-%02d SYSTEM CLOSED DATE\n",
			xsystem.xclosedate.year4, xsystem.xclosedate.month, xsystem.xclosedate.day );
	}

	if ( MyImport.xprevious.month > 0 )
	{
		printf ( "%04d-%02d-%02d PREVIOUS IMPORT DATE\n",
			MyImport.xprevious.year4, MyImport.xprevious.month, MyImport.xprevious.day );
	}

	printf ( "---------------------------------------------------------------\n" );
	printf ( "PROBLEM  TRX-DATE   REF      PAYEE                       AMOUNT\n" );
	printf ( "---------------------------------------------------------------\n" );
	//        x8xxxxxx yyyy.mm.dd x8xxxxxx x25of40xxxxxxxxxxxxxxxxxx 99999.99
}

void SaveTransaction ()
{
	int			Affected;
	long		ThisTrxNumber, ThatTrxNumber;
	short		ThisSeqNumber;
	long		NewBalance;

	if ( NewTrxd.xamount == 0 )
	{
		if ( ReportSkips == 1 )
		{
			ReportCount++;
			if ( ReportCount == 1 )
			{
				PrintHeader ();
			}

			printf ( "Skipping zero amount\n" );
		}
		return;
	}

	if ( MyImport.xreverse[0] == 'Y' )
	{
		NewTrxd.xamount = 0 - NewTrxd.xamount;
		printf ( "Reversing sign\n" );
	}

	if ( MyAccount.xaccttype[0] == 'L' && MyImport.xpayments[0] == 'N' && NewTrxd.xamount < 0L )
	{
		if ( ReportSkips == 1 )
		{
			ReportCount++;
			if ( ReportCount == 1 )
			{
				PrintHeader ();
			}

			printf ( "Skipping liability payment, amount %ld\n", NewTrxd.xamount );
		}
		return;
	}
	
	if ( MyAccount.xaccttype[0] == 'A' && MyImport.xpayments[0] == 'N' && NewTrxd.xamount > 0L )
	{
		if ( ReportSkips == 1 )
		{
			ReportCount++;
			if ( ReportCount == 1 )
			{
				PrintHeader ();
			}

			printf ( "Skipping asset deposit, amount %ld\n", NewTrxd.xamount );
		}
		return;
	}
	
	DateOkay = 0;
	if ( CompareDateval ( &NewTrxh.xtrxdate, &xsystem.xclosedate ) <= 0 )
	{
		DateOkay = 1;
		DateClosed++;
	}
	else if ( CompareDateval ( &NewTrxh.xtrxdate, &MyImport.xprevious ) <= 0 )
	{
		DateOkay = 2;
		DatePrevious++;
	}

	if ( DateOkay != 0 )
	{
		DateCount++;
		if ( ReportSkips == 1 )
		{
			ReportCount++;
			if ( ReportCount == 1 )
			{
				PrintHeader ();
			}

			printf ( "%-8.8s %04d-%02d-%02d %-8.8s %-25.25s %8.2f\n",
				DateOkay == 1 ? "CLOSED" : "PREVIOUS",
				NewTrxh.xtrxdate.year4, NewTrxh.xtrxdate.month, NewTrxh.xtrxdate.day,
				NewTrxh.xrefnum, NewTrxd.xpayee, (double) NewTrxd.xamount / 100.0 );
		}
		return;
	}

	Stripper ( NewTrxh.xrefnum, "\'\"" );
	Stripper ( NewTrxd.xpayee,  "\'\"" );

	BetterRefnum ();

	ThisTrxNumber = 0L;
	ThisSeqNumber = 0;
	if ( FindTrx ( &ThisTrxNumber, &ThisSeqNumber ) == 0 )
	{
		ThisTrxNumber = xsystem.xtrxnext;
		ThisSeqNumber = 1;

		sprintf ( StatementOne,
"insert into trxh (trxnum,status,refnum,trxdate) values ( %ld, %d, '%-.8s', '%04d-%02d-%02d' )",
							ThisTrxNumber,
							AutoStatus ? STATUS_OPEN : STATUS_ACCEPT,
							NewTrxh.xrefnum,
							NewTrxh.xtrxdate.year4, NewTrxh.xtrxdate.month, NewTrxh.xtrxdate.day );
		
		Affected = dbyInsert ( "import", &MySql, StatementOne, 0, LOGFILENAME );

		if ( Affected == 0 )
		{
			SaveError ( "insert new transaction header" );
			SaveError ( "import processing canceled" );

			AcctErrorCount++;

			if ( ReportSkips == 1 )
			{
				printf ( "FAILED: ThisTrxNumber %ld ThisSeqNumber %d\n", ThisTrxNumber, ThisSeqNumber );
				printf ( "FAILED: %s\n", StatementOne );
			}
			return;
		}
	}

	sprintf ( StatementOne,
"insert into trxd (trxnum,seqnum,acctnum,payee,amount) values ( %ld,   %d,    %d,     '%-.40s', %ld )",
		ThisTrxNumber,
		ThisSeqNumber,
		ImportAcctnum,
		NewTrxd.xpayee,
		NewTrxd.xamount );
		
	Affected = dbyInsert ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	if ( Affected > 0 )
	{
		ImportCount++;

		if ( Verbose )
		{
			printf ( "Added trx %ld to %d for %ld\n", ThisTrxNumber, ImportAcctnum, NewTrxd.xamount );
		}

		if ( ThisTrxNumber == xsystem.xtrxnext )
		{
			LastTrx = xsystem.xtrxnext;
			xsystem.xtrxnext++;
		}

		if ( CompareDateval ( &MaxDate, &NewTrxh.xtrxdate ) < 0 )
		{
			memcpy ( &MaxDate, &NewTrxh.xtrxdate, sizeof(DATEVAL) );

			if ( Verbose == 1 )
			{
				printf ( "MAX DATE %04d-%02d-%02d\n", MaxDate.year4, MaxDate.month, MaxDate.day );
			}
		}

		/*--------------------------------------------------------------
			update account.currbal
		--------------------------------------------------------------*/
		sprintf ( StatementOne,
			"update account set currbal = currbal + %ld where acctnum = %d",
				NewTrxd.xamount, ImportAcctnum );

		Affected = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
		if ( Affected == 0 )
		{
			SaveError ( "update account current balance failed" );
			AcctErrorCount++;

			if ( Verbose == 1 )
			{
				printf ( "FAILED: %s\n", StatementOne );
			}
		}

		if ( ThisSeqNumber == 1 )
		{
			if (( LoadOneTrxh ( ThisTrxNumber                ) == 1 ) &&
				( LoadOneTrxd ( ThisTrxNumber, ThisSeqNumber ) == 1 ))
			{
				memcpy ( &NewTrxh, &xtrxh, sizeof(XTRXH) );
				memcpy ( &NewTrxd, &xtrxd, sizeof(XTRXD) );

				if ( FindRepeating ( 1, &NewTrxh, &NewTrxd, &ThatTrxNumber ) == 1 )
				{
					if ( Verbose == 1 )
					{
						printf ( "%ld: found repeating by exact amount, trxnum %ld\n", ThisTrxNumber, ThatTrxNumber );
					}
				}
				else if ( FindRepeating ( 2, &NewTrxh, &NewTrxd, &ThatTrxNumber ) == 1 )
				{
					if ( Verbose == 1 )
					{
						printf ( "%ld: found repeating by MATCHING payee and refnum, trxnum %ld\n",  ThisTrxNumber,ThatTrxNumber );
					}
				}
				else if ( FindRepeating ( 3, &NewTrxh, &NewTrxd, &ThatTrxNumber ) == 1 )
				{
					if ( Verbose == 1 )
					{
printf ( "%ld: found repeating by SIMILAR payee (%.2f) and refnum, trxnum %ld\n", ThisTrxNumber, dblScore, ThatTrxNumber );
					}
				}
			}
		}

		NewBalance = CalcTrxBalance ( ThisTrxNumber );

		if ( NewBalance != 0 )
		{
			sprintf ( StatementOne, "update trxh set status = %d where trxnum = %ld", STATUS_BALANCE, ThisTrxNumber );
			dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
		}
	}
	else
	{
		SaveError ( "insert transaction failed" );
		AcctErrorCount++;

		if ( Verbose == 1 )
		{
			printf ( "FAILED: %s\n", StatementOne );
		}
	}
	if ( Verbose )
	{
		printf ( "--------------------------------------------------------------------------------\n" );
	}
}
