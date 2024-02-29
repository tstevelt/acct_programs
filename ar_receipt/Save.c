/*----------------------------------------------------------------------------
	Program : Save.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Apply cash to invoice[s] and update invoice header.
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

void Save ()
{
	int		ndx;
	long	InvoiceTotal;

	/*----------------------------------------------------------
		save invoice information
	----------------------------------------------------------*/
	for ( ndx = 0; ndx < StoreCount; ndx++ )
	{
		if ( StoreArray[ndx].Discount == 0 && StoreArray[ndx].Payment == 0 )
		{
			continue;
		}

		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", StoreArray[ndx].InvoiceNumber );
		InvoiceTotal = dbySelectSumLong ( &MySql, "arinvl", "amount", WhereClause, LOGFILENAME );

		if ( InvoiceTotal <= StoreArray[ndx].Discount + StoreArray[ndx].Payment )
		{
			xarinvh.xstatus[0] = 'P';
		}
		else
		{
			xarinvh.xstatus[0] = 'O';
		}

		sprintf ( StatementOne,
"update arinvh set discount = %ld, payment = %ld, refnum = '%s', paydate = '%04d-%02d-%02d', status = '%c' where id = %ld",
				StoreArray[ndx].Discount,
				StoreArray[ndx].Payment,
				CheckNumber,
				CheckDate.year4, 
				CheckDate.month, CheckDate.day,
				xarinvh.xstatus[0],
				StoreArray[ndx].InvoiceNumber );

		dbyUpdate ( "ar_receipt", &MySql, StatementOne, 0, LOGFILENAME );
	}
}
