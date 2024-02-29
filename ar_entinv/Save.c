/*----------------------------------------------------------------------------
	Program : Save.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update invoice header and detail
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_entinv.h"

void Save ()
{
	int		ndx;

	/*----------------------------------------------------------
		save header information
	----------------------------------------------------------*/
	sprintf ( StatementOne,
		"update arinvh set customer = %ld, invdate = '%04d-%02d-%02d', terms = '%s', ponum = '%s' where id = %ld",
			xarinvh.xcustomer,
			xarinvh.xinvdate.year4, 
			xarinvh.xinvdate.month, xarinvh.xinvdate.day,
			xarinvh.xterms,
			xarinvh.xponum,
			xarinvh.xid );

	dbyUpdate ( "ar_entinv", &MySql, StatementOne, 0, LOGFILENAME );

	/*----------------------------------------------------------
		save line information
	----------------------------------------------------------*/

	for ( ndx = 0; ndx < StoreCount; ndx++ )
	{
		sprintf ( StatementOne,
			"update arinvl set descr = '%s', amount = %ld where id = %ld and invoice = %ld",
				StoreArray[ndx].Descr,
				StoreArray[ndx].Amount,
				StoreArray[ndx].LineNumber,
				xarinvh.xid );

		dbyUpdate ( "ar_entinv", &MySql, StatementOne, 0, LOGFILENAME );
	}
}
