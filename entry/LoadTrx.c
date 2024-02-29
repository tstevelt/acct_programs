/*----------------------------------------------------------------------------
	Program : LoadTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load one transaction header and all its detail
	Return  : 
----------------------------------------------------------------------------*/


#include	"entry.h"

int LoadTrx ( long Trxnum )
{
	DBY_QUERY		*QueryOne;

	TrxCount = 0;

	sprintf ( StatementOne,
		"select %s from trxh, trxd where trxh.trxnum = trxd.trxnum and trxh.trxnum = %ld order by seqnum",
					FLDS_TRXV, Trxnum );

	QueryOne = dbySelect ( "accounts", &MySql, StatementOne, LOGFILENAME );

	while (( QueryOne->EachRow = mysql_fetch_row ( QueryOne->Result )) != NULL )
	{
		TrxCount = safe_atoi ( QueryOne->EachRow [ TRXV_seqnum ] );

		if ( TrxCount < 1 || TrxCount > MAXSEQNO )
		{
			SaveError ( "invalid transaction sequence number" );
			return ( -1 );
		}

		xtrxh.xtrxnum = safe_atoi ( QueryOne->EachRow [ TRXV_trxnum ] );
		xtrxh.xstatus = safe_atoi ( QueryOne->EachRow [ TRXV_status ] );
		sprintf ( xtrxh.xrefnum, "%s", QueryOne->EachRow [ TRXV_refnum ] );
		StringToDateval ( QueryOne->EachRow [ TRXV_trxdate ], &xtrxh.xtrxdate );

		TrxArray[TrxCount].d.xtrxnum = safe_atoi ( QueryOne->EachRow [ TRXV_trxnum ] );
		TrxArray[TrxCount].d.xseqnum = safe_atoi ( QueryOne->EachRow [ TRXV_seqnum ] );
		TrxArray[TrxCount].d.xacctnum = safe_atol ( QueryOne->EachRow [ TRXV_acctnum ] );
		sprintf ( TrxArray[TrxCount].d.xpayee, "%s", QueryOne->EachRow [ TRXV_payee ] );
		TrxArray[TrxCount].d.xamount = safe_atol(QueryOne->EachRow[TRXV_amount]);

		if ( LoadOneAccount ( TrxArray[TrxCount].d.xacctnum ) == 1 )
		{
			snprintf ( TrxArray[TrxCount].c.xacctname, sizeof ( TrxArray[TrxCount].c.xacctname ), 
					"%s", xaccount.xacctname );
		}
		else
		{
			TrxArray[TrxCount].c.xacctname[0] = '\0';
		}
	}

	return ( TrxCount );

}
