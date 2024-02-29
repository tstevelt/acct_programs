/*----------------------------------------------------------------------------
	Program : Find.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Find open invoices
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_receipt.h"

static int EachInvoice ( XARINVH *ptrArinvh )
{
	return ( 0 );
}

void Find ()
{
	snprintf ( WhereClause, sizeof(WhereClause), "customer = %ld and status = 'O'", xarcust.xid );

	if ( LoadArinvhCB ( WhereClause, "id", &xarinvh, (int(*)()) EachInvoice, 0 ) < 1 )
	{
		SaveError ( "No open invoices found for this customer" );
		xarinvh.xid = 0;
		return;
	}


}
