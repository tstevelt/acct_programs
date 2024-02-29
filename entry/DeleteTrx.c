/*----------------------------------------------------------------------------
	Program : DeleteTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Delete transaction detail and header, updating accounts.
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
