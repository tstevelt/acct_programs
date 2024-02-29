/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
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

static int DebugChkInput = 0;

int ChkInput ()
{
	int		rv = 0;
	int		ndx;
	long	CheckTotal;
	long	DistroTotal;
	
	switch ( RunMode )
	{
		case MODE_START:
		case MODE_QUIT:
			break;
		case MODE_FIND:
			if ( xarcust.xid == 0 )
			{
				SaveError ( "Enter customer number to find" );
				RunMode = MODE_START;
				break;
			}
			snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarcust.xid );
			if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
			{
				SaveError ( "Unknown customer number" );
				xarcust.xid = 0;
				RunMode = MODE_START;
			}
			break;

		case MODE_SAVE:
			CheckTotal = 0.5 + CheckAmount * 100.0;
			DistroTotal = 0;
			for ( ndx = 0; ndx < StoreCount; ndx++ )
			{
				if ( StoreArray[ndx].Payment + StoreArray[ndx].Discount > StoreArray[ndx].Total )
				{
					sprintf ( msgbuf, "Distro amount %.2f overpayment on invoice %ld", 
						(double) (StoreArray[ndx].Payment + StoreArray[ndx].Discount) / 100.0, StoreArray[ndx].InvoiceNumber );
					SaveError ( msgbuf );
					RunMode = MODE_FIND;
					return ( -1 );
				}

				DistroTotal = DistroTotal + StoreArray[ndx].Payment + StoreArray[ndx].Discount;
				if ( DebugChkInput )
				{
					printf ( "%ld %ld %ld %ld<br>\n",  
						StoreArray[ndx].Payment, StoreArray[ndx].Discount, DistroTotal, CheckTotal );
				}
			}
			if ( DistroTotal != CheckTotal )
			{
				sprintf ( msgbuf, "Distro amount %.2f doee not match check", (double) DistroTotal / 100.0 );
				SaveError ( msgbuf );
				RunMode = MODE_FIND;
			}
			break;

	}
	return ( rv );
}
