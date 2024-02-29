/*----------------------------------------------------------------------------
	Program : Save.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update invoice header and detail
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
