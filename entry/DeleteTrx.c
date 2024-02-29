/*----------------------------------------------------------------------------
	Program : DeleteTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Delete transaction detail and header, updating accounts.
	Return  : 
----------------------------------------------------------------------------*/

#include	"entry.h"


int DeleteTrx ( long Trxnum )
{
	int		MyRv;
	int		xt;

	LoadTrx ( Trxnum );

	if ( TrxCount < 1 )
	{
		return ( -1 );
	}

	for ( xt = 1; xt <= TrxCount; xt++ )
	{
		sprintf ( StatementOne,
			"update account set currbal = currbal - %ld where acctnum = %ld",
				TrxArray[xt].d.xamount, TrxArray[xt].d.xacctnum );

		MyRv = dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );

		if ( MyRv == 1 )
		{
			sprintf ( StatementOne,
				"delete from trxd where trxnum = %ld and seqnum = %d",
					TrxArray[xt].d.xtrxnum, TrxArray[xt].d.xseqnum );

			MyRv = dbyDelete ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
			
			if ( MyRv != 1 )
			{
				SaveError ( "can not delete a transaction detail record" );
				return ( -2 );
			}
		}
		else
		{
			SaveError ( "can not update account/category" );
			return ( -3 );
		}
	}

	sprintf ( StatementOne, "delete from trxh where trxnum = %ld", Trxnum );

	MyRv = dbyDelete ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
		
	if ( MyRv != 1 )
	{
		SaveError ( "can not delete a transaction header record" );
		return ( -2 );
	}

	return ( 0 );
}
