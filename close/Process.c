/*----------------------------------------------------------------------------
	Program : Process.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Make sure transactions can be closed. If so, set status and date.
	Return  : 
----------------------------------------------------------------------------*/

#define DEBUGGER
#undef DEBUGGER

#include	"close.h"

static int DebugReport = 0;

static int SumByType ( char AccountType, long *TypeTotal, DATEVAL startdate, DATEVAL enddate  )
{
	char		Fragment[40];
	DBY_QUERY	*QueryOne;
	DBY_QUERY	*QueryTwo;
	long		open_balance;
	long		account_total;
	long		type_total;
	int			Count;
	int			IncludeOpen;

	*TypeTotal = 0L;

	sprintf ( StatementOne, "select %s from account where accttype = '%c' order by acctnum",
				FIELDS_ACCOUNT, AccountType );

	if ( DebugReport )
	{
		printf ( "%s<br>\n", StatementOne );
	}

	type_total= 0L;
	Count = 0;

	QueryOne = dbySelect ( "rptgl", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		open_balance = safe_atol ( QueryOne->EachRow[ACCOUNT_openbal] );

		sprintf ( StatementTwo,
			"select sum(amount) from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %s and trxdate <= '%04d-%02d-%02d'",
				QueryOne->EachRow[ACCOUNT_acctnum],
				enddate.year4, enddate.month, enddate.day );

		switch ( AccountType )
		{
			case 'A':
			case 'L':
			case 'E':
				IncludeOpen = 1;
				break;

			case 'I':
			case 'X':
				sprintf ( Fragment,
					" and trxdate > '%04d-%02d-%02d'", startdate.year4,startdate.month, startdate.day );
				strcat ( StatementTwo, Fragment );
				IncludeOpen = 0;
				break;

			default:
				SaveError ( "Invalid Account Type" );
				return ( 0 );
		}

		if ( DebugReport )
		{
			printf ( "%s<br>\n", StatementTwo );
		}

		QueryTwo = dbySelect ( "rptgl", &MySql, StatementTwo, LOGFILENAME );

		if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) == NULL )
		{
			account_total = 0L;
		}
		else
		{
			account_total = safe_atol ( QueryTwo->EachRow[0] );
		}

		if ( IncludeOpen )
		{
			account_total += open_balance;
		}

		Count++;

		type_total += account_total;

	}

	*TypeTotal = type_total;

	return ( Count );
}

void Process ()
{
	DBY_QUERY	*Query;
	int			Count, OutOfBalance, Affected;
	long		TrxBalance, ReportTotal;
	long		Asset, Liability, Equity;
	long		Income, Expense;
	char		xbuf[80];
	double		dblValue;

#ifdef DEBUGGER
	printf ( "Processing %02d/%02d/%04d ...<br>\n",
				closedate.month, closedate.day, closedate.year4 );
#endif

	if ( xsystem.xreconcile[0] == 'Y' )
	{
		/*--------------------------------------------------------------
			system requires reconciliation of all transactions 
			prior to closing.  check it out.
		--------------------------------------------------------------*/
		Count = 0;

		sprintf ( StatementOne, 
			"select count(*) from trxh where status < %d and trxdate <= '%04d-%02d-%02d'",
				STATUS_RECONCILED, closedate.year4, closedate.month, closedate.day );

		if (( Query = dbySelect ( "close", &MySql, StatementOne, LOGFILENAME )) != NULL )
		{
			if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
			{
				Count = safe_atoi ( Query->EachRow[0] );
			}
		}

		if ( Count > 0 )
		{
			snprintf ( xbuf, sizeof(xbuf),
					"there are %d un-reconciled transactions prior to close date", Count );
			SaveError ( xbuf  );
			return;
		}
#ifdef DEBUGGER
		else
		{
			printf ( "Checked %d for reconciled, all okay<br>\n", Count );
		}
#endif
	}

	/*--------------------------------------------------------------
		make sure all transactions which are about to be 
		closed are in-balance.
	--------------------------------------------------------------*/
	Count = 0;
	OutOfBalance = 0;

	sprintf ( StatementOne, 
		"select trxnum, status from trxh where status < %d and trxdate <= '%04d-%02d-%02d'",
			STATUS_CLOSED, closedate.year4, closedate.month, closedate.day );

	if (( Query = dbySelect ( "close", &MySql, StatementOne, LOGFILENAME )) == NULL )
	{
		SaveError ( "check balance query failed"  );
		return;
	}

	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		Count++;
		xtrxh.xtrxnum = safe_atol ( Query->EachRow[0] );
		xtrxh.xstatus = safe_atol ( Query->EachRow[1] );
		if (( TrxBalance = CalcTrxBalance ( xtrxh.xtrxnum )) != 0 )
		{
			printf ( "trx %ld is out of balance.<br>\n", xtrxh.xtrxnum );
			OutOfBalance++;
		}

		if ( TrxBalance != 0L && xtrxh.xstatus != STATUS_BALANCE )
		{
			snprintf ( StatementTwo, sizeof(StatementTwo),
				"update trxh set status = %d where trxnum = %ld",
					STATUS_BALANCE, xtrxh.xtrxnum );

			if ( dbyUpdate ( "close", &MySql, StatementTwo, 0, LOGFILENAME ) != 1 )
			{
				snprintf ( xbuf, sizeof(xbuf),
						"can not update status on transaction %ld.  close canceled.",
											xtrxh.xtrxnum );
				SaveError ( xbuf  );
				return;
			}
		}
		else if ( TrxBalance == 0L && xtrxh.xstatus == STATUS_BALANCE )
		{
			snprintf ( StatementTwo, sizeof(StatementTwo),
				"update trxh set status = %d where trxnum = %ld",
					STATUS_OPEN, xtrxh.xtrxnum );

			if ( dbyUpdate ( "close", &MySql, StatementTwo, 0, LOGFILENAME ) != 1 )
			{
				snprintf ( xbuf, sizeof(xbuf),
						"can not update status on transaction %ld.  close canceled.",
											xtrxh.xtrxnum );
				SaveError ( xbuf  );
				return;
			}
		}
	}

	if ( OutOfBalance > 0 )
	{
		snprintf ( xbuf, sizeof(xbuf),
			"checked %d transactions, %d are out-of-balance", Count, OutOfBalance );
		SaveError ( xbuf  );
		return;
	}
#ifdef DEBUGGER
	else
	{
		printf ( "Checked %d for out-of-balance, all okay<br>\n", Count );
	}
#endif

	/*----------------------------------------------------------
		Closing Method.
		* Manual, then Balance Sheet account need to net
		  to zero as of new close date.
		* Auto, then need to calc net margin between 
		  last and new close date and add transation.
	----------------------------------------------------------*/
	switch ( xsystem.xclosemethod[0] )
	{
		case 'M':
			SumByType ( 'A', &Asset, closedate, closedate );
			SumByType ( 'L', &Liability, closedate, closedate );
			SumByType ( 'E', &Equity, closedate, closedate );
			ReportTotal = Asset - Liability - Equity;
			if ( ReportTotal != 0 )
			{
				SaveError ( "Manual close method requires zero balance." );
				sprintf ( xbuf, "Balance as of %d-%d-%d is %.2f.",
						closedate.month, closedate.day, closedate.year2, (double) ReportTotal / 100.0 );
				SaveError ( xbuf );
				return;
			}
			break;
		case 'A':
// fixit
			SumByType ( 'I', &Income, xsystem.xclosedate, closedate );
			SumByType ( 'X', &Expense, xsystem.xclosedate, closedate );
			ReportTotal = Income - Expense;
#ifdef DEBUG
sprintf ( xbuf, "Net Income as from >%d-%d-%d to <=%d-%d-%d is %.2f (%ld %ld %ld).",
						xsystem.xclosedate.month, xsystem.xclosedate.day, xsystem.xclosedate.year2, 
						closedate.month, closedate.day, closedate.year2, 
						(double) ReportTotal / 100.0,
						Income, Expense, ReportTotal );
SaveError ( xbuf );
return;
#endif
			if ( ReportTotal != 0 )
			{
				dblValue = ReportTotal / 100.0;

				AddDoubleEntryTrx ( xsystem.xretainequity, dblValue, xsystem.xretainincome, 0.0 - dblValue, 
					"CLOSE", xsystem.xname, closedate );

				Count++;
			}
			break;
		default:
			SaveError ( "Company closing method not set, see setup screen." );
			return;
	}

	/*--------------------------------------------------------------
		everything kool, close 'em.
	--------------------------------------------------------------*/
	sprintf ( StatementOne, 
		"update trxh set status = %d where status < %d and trxdate <= '%04d-%02d-%02d'",
			STATUS_CLOSED, STATUS_CLOSED, closedate.year4, closedate.month, closedate.day );

	Affected = dbyUpdate ( "close", &MySql, StatementOne, 0, LOGFILENAME );

	if ( Affected != Count )
	{
		snprintf ( xbuf, sizeof(xbuf),
			"updated %d of %d transactions, system close date not changed", Affected, Count );
		SaveError ( xbuf  );
		return;
	}

	/*--------------------------------------------------------------
		update system close date
	--------------------------------------------------------------*/
	sprintf ( StatementOne, 
		"update system set closedate = '%04d-%02d-%02d'",
			closedate.year4, closedate.month, closedate.day );

	Affected = dbyUpdate ( "close", &MySql, StatementOne, 0, LOGFILENAME );

	if ( Affected != 1 )
	{
		SaveError ( "could not update system close date"  );
		return;
	}

	LoadSystem ();
}
