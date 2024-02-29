/*----------------------------------------------------------------------------
	Program : Update.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: stamp transaction status as RECONCILED
	Return  : 
----------------------------------------------------------------------------*/


#include	"reconcile.h"

void Update ()
{
	int		xt;

	for ( xt = 0; xt < TrxCount; xt++ )
	{
		sprintf ( StatementOne,
			"update trxh set status = %d where trxnum = %ld",
				STATUS_RECONCILED, TrxArray[xt].d.xtrxnum );

		dbyUpdate ( "reconcile", &MySql, StatementOne, 0, LOGFILENAME );
	}
}
