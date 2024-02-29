/*----------------------------------------------------------------------------
	Program : Register.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint the transactions for an account.
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

void Register ()
{
	DBY_QUERY		*QueryOne;
	DBY_QUERY		*QueryTwo;
	int				MyCount =  0;
	int				TrxCount;
	XTRXH			ThisTrxh;
	XTRXD			ThisTrxd;
	short			FlipFlop = 0;
	char			ColorString [ 20 ];
	char			AC_Flag;
	char			MemoString [ 41 ];
	char			DateCriteria [ 100 ];
	DATEVAL			sdt, edt;

	webStartJava ();
	printf ( "function EditTrx ( Trxnum )\n" );
	printf ( "{\n" );
	printf ( "var command = 'entry.cgi?edit=' + Trxnum;\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=750,height=500,resizable=1,menubar=0,scrollbars=1';\n" );
	printf ( "window.open(command,'editwin',options);\n" );
	printf ( "}\n" );
	webEndJava ();

	printf ( "<table class='AppWide'>\n" );

	AC_Flag = '?';
	if ( LoadOneAccount ( xaccount.xacctnum ) == 1 )
	{
		switch ( xaccount.xaccttype[0] )
		{
			case 'A':
			case 'L':
			case 'E':
				AC_Flag = 'A';
				break;
			case 'I':
			case 'X':
				AC_Flag = 'C';
				break;
		}

		printf ( "<tr>\n" );


		printf ( "<input type='hidden' name='register' value='%ld'>\n", 
									xaccount.xacctnum );

		printf ( "<th colspan='4' align='center'>%ld&nbsp;%s - %s</th>\n", 
									xaccount.xacctnum, xaccount.xacctname, TimeFrameDescr(xaccount.xdaterange) );
		printf ( "</tr>\n" );

	}

	/*-----------------------------------------------------------------------------
		payee and memo are same field, trxd.payee.

		for asset, liablity (and equity), first line is seq num 1
		if count(*) for this transaction > 2, then memo is <split>,
		other wise it is trxd.payee for seqnum 2.

			date			ref		payee 				amount
			e (trx/seq)				memo			

		for income 

			date			ref		payee 				amount
			e (trx/seq)				memo			

		for expense

			date			ref		payee 				amount
			e (trx/seq)				memo			
	-----------------------------------------------------------------------------*/

	if ( TimeFrameDates ( xaccount.xdaterange, &sdt, &edt ) == 0 )
	{
#ifdef DO_NOT_LOOK_AHEAD_FOR_SOME_STRANGE_REASON
		snprintf ( DateCriteria, sizeof(DateCriteria), 
			"and trxdate >= '%04d-%02d-%02d' and trxdate <= '%04d-%02d-%02d'",
				sdt.year4, sdt.month, sdt.day,
				edt.year4, edt.month, edt.day );
#else
		snprintf ( DateCriteria, sizeof(DateCriteria), 
			"and trxdate >= '%04d-%02d-%02d'",
				sdt.year4, sdt.month, sdt.day );
#endif

	}
	else
	{
		DateCriteria[0] = '\0';
	}

	sprintf ( StatementOne,
"select %s from trxh, trxd where trxh.trxnum = trxd.trxnum and acctnum = %ld %s order by trxdate, trxd.trxnum, seqnum",
			FLDS_TRXV, xaccount.xacctnum, DateCriteria );

	QueryOne = dbySelect ( "accounts", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		MyCount++;

		StringToDateval ( QueryOne->EachRow [ TRXV_trxdate ], &ThisTrxh.xtrxdate );
		sprintf ( ThisTrxh.xrefnum, "%s", QueryOne->EachRow [ TRXV_refnum ] );
		ThisTrxh.xstatus = safe_atoi(QueryOne->EachRow [ TRXV_status ]);

		ThisTrxd.xtrxnum = safe_atoi ( QueryOne->EachRow [ TRXV_trxnum ] );
		ThisTrxd.xseqnum = safe_atoi ( QueryOne->EachRow [ TRXV_seqnum ] );
		sprintf ( ThisTrxd.xpayee, "%s", QueryOne->EachRow [ TRXV_payee ] );
		ThisTrxd.xamount = safe_atol(QueryOne->EachRow [ TRXV_amount ] );

		if ( AC_Flag == 'A' )
		{
			sprintf ( StatementTwo,
				"select count(*) from trxd where trxnum = %ld", ThisTrxd.xtrxnum );

			QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );

			if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
			{
				TrxCount = safe_atoi ( QueryTwo->EachRow [ 0 ] );
			}
			else
			{
				TrxCount = 1;
			}
			
			switch ( TrxCount )
			{
				case 1:
					nsStrcpy ( MemoString, "&nbsp;" );
					break;
				case 2:
					sprintf ( StatementTwo,
						"select payee from trxd where trxnum = %ld and seqnum = 2", ThisTrxd.xtrxnum );

					QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );

					if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
					{
						snprintf ( MemoString, sizeof(MemoString), "%s", QueryTwo->EachRow [ 0 ] );
					}
					else
					{
						nsStrcpy ( MemoString, "&nbsp;" );
					}
					break;
				default:
					nsStrcpy ( MemoString, "[split]" );
					break;
			}

		}
		else
		{
			nsStrcpy ( MemoString, "&nbsp;" );
			if ( ThisTrxd.xseqnum > 1 )
			{
				sprintf ( StatementTwo,
					"select payee from trxd where trxnum = %ld and seqnum = 1", ThisTrxd.xtrxnum );

				QueryTwo = dbySelect ( "accounts", &MySql, StatementTwo, LOGFILENAME );

				if (( QueryTwo->EachRow = mysql_fetch_row ( QueryTwo->Result )) != NULL )
				{
					snprintf ( MemoString, sizeof(MemoString), "%s", QueryTwo->EachRow [ 0 ] );
				}
			}
		}

		if ( MyCount == 1 )
		{
			printf ( "<tr>\n" );
			printf ( "<th align='left'>date</th>\n" );
			printf ( "<th align='left'>ref</th>\n" );
			printf ( "<th align='left'>payee</th>\n" );
			printf ( "<th align='left'>&nbsp;</th>\n" );
			printf ( "<th align='left'>amount</th>\n" );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<th align='left'>trx/seq</th>\n" );
			printf ( "<th align='left'>&nbsp;</th>\n" );
			printf ( "<th align='left' colspan='2'>memo</th>\n" );
			printf ( "<th align='left'>&nbsp;</th>\n" );
			printf ( "</tr>\n" );
		}
		
		if ( FlipFlop == 0 )
		{
			FlipFlop = 1;
			nsStrcpy ( ColorString, COLOR_FLIP_FLOP_ODD );
		}
		else
		{
			FlipFlop = 0;
			nsStrcpy ( ColorString, COLOR_FLIP_FLOP_EVEN );
		}

		/*--------------------------------------------------------------
			first row
		--------------------------------------------------------------*/
		printf ( "<tr bgcolor='%s'>\n", ColorString );

		printf ( "<td>%02d/%02d/%02d</td>\n",
				ThisTrxh.xtrxdate.month, ThisTrxh.xtrxdate.day, ThisTrxh.xtrxdate.year2 );

		printf ( "<td>%s</td>\n", ThisTrxh.xrefnum );

		printf ( "<td>%s</td>\n", ThisTrxd.xpayee );

		printf ( "<td>%s</td>\n", StatusDescr(ThisTrxh.xstatus, 1) );

		printf ( "<td" );
		switch ( ThisTrxh.xstatus )
		{
			case STATUS_PREENT: printf ( " bgcolor='%s'", COLOR_PREENT ); break;
			case STATUS_ACCEPT: printf ( " bgcolor='%s'", COLOR_ACCEPT ); break;
			case STATUS_BALANCE: printf ( " bgcolor='%s'", COLOR_BALANCE ); break;
		}
		printf ( " align='right'>%.2f</td>\n", (double) ThisTrxd.xamount / 100.0 );

		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			second row
		--------------------------------------------------------------*/
		printf ( "<tr bgcolor='%s'>\n", ColorString );

		printf ( "<td>" );

		/*--------------------------------------------------------------
			printf ( "<a href='#' onClick='javascript:EditTrx(%ld);'>%ld/%d</a>",
						ThisTrx.xtrxnum, ThisTrx.xtrxnum, ThisTrx.xseqnum );
		--------------------------------------------------------------*/
		printf ( "<input type='button' value='%c' ", ThisTrxh.xstatus == STATUS_CLOSED ? 'v' : 'e' );
		printf ( "onClick='javascript:EditTrx(%ld);'>\n", ThisTrxd.xtrxnum );
#ifdef ACCT_VERBOSE
		printf ( "&nbsp;(%ld/%d)", ThisTrxd.xtrxnum, ThisTrxd.xseqnum );
#endif
		printf ( "</td>\n" );

		printf ( "<td>&nbsp;</td>\n" );

		printf ( "<td colspan='2'>%s</td>\n",  MemoString );

		printf ( "<td>&nbsp;</td>\n" );

		printf ( "</tr>\n" );

	}

	printf ( "<tr>\n" );
	printf ( "<td colspan='4' align='center'>" );

	printf ( "<input type='button' value='list' " );
	printf ( "onClick='javascript:window.open(\"accounts.cgi?ac=%c\",\"_self\");'>\n", AC_Mode );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>" );
}
