/*----------------------------------------------------------------------------
	Program : Find.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Find open invoices
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
