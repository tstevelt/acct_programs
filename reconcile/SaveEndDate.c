/*----------------------------------------------------------------------------
	Program : SaveEndDate.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update account record recon date, balance
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

#include	"reconcile.h"

void SaveEndDate ()
{
	if ( EndDate.month == 0 )
	{
		return;
	}

	sprintf ( StatementOne,
"update account set recondate = '%04d-%02d-%02d', reconbal = %ld where acctnum = %ld and recondate < '%04d-%02d-%02d'",
				EndDate.year4, EndDate.month, EndDate.day, 
				EndBalance,
				xaccount.xacctnum,
				EndDate.year4, EndDate.month, EndDate.day );

//printf ( "%s<br>\n", StatementOne );

	dbyUpdate ( "reconcile", &MySql, StatementOne, 0, LOGFILENAME );

}
