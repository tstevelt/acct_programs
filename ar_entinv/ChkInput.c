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

#include	"ar_entinv.h"

int ChkInput ()
{
	int		rv = 0;
	
	switch ( RunMode )
	{
		case MODE_START:
		case MODE_QUIT:
			break;
		case MODE_FIND:
//			if ( xarinvh.xid == 0 )
//			{
//				SaveError ( "Enter invoice number to find" );
//				RunMode = MODE_START;
//			}
			break;
		case MODE_SAVE:
			break;
		case MODE_ADD_HEAD:
			if ( xarinvh.xcustomer == 0 )
			{
				SaveError ( "Enter customer number to add invoice" );
				RunMode = MODE_START;
				break;
			}
			snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
			if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
			{
				SaveError ( "Unknown customer number" );
				xarinvh.xcustomer = 0;
				RunMode = MODE_START;
			}
			break;
		case MODE_ADD_LINE:
			break;
	}
	return ( rv );
}
