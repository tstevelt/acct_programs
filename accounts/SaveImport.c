/*----------------------------------------------------------------------------
	Program : SaveImport.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update import record
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

#include	"accounts.h"

int SaveImport ( int AccountNumber )
{
	int		rv = 0;

#ifdef VERSION_ONE
	sprintf ( StatementOne, 
"update import set website = '%s', login = '%s', previous = '%04d-%02d-%02d', payments = '%c', autoparse = '%c' where acctnum = %d",
			ximport.xwebsite,
			ximport.xlogin,
			ximport.xprevious.year4, ximport.xprevious.month, ximport.xprevious.day,
			ximport.xpayments[0],
			ximport.xautoparse[0],
			AccountNumber );
#else
	sprintf ( StatementOne, 
"update import set website = '%s', login = '%s', previous = '%04d-%02d-%02d', payments = '%c', reverse = '%c' where acctnum = %d",
			ximport.xwebsite,
			ximport.xlogin,
			ximport.xprevious.year4, ximport.xprevious.month, ximport.xprevious.day,
			ximport.xpayments[0],
			ximport.xreverse[0],
			AccountNumber );
#endif
	
// printf ( "%s<br>\n", StatementOne );
	rv = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	
#ifdef VERSION_ONE
	if ( ximport.xautoparse[0] == 'Y' )
	{
		sprintf ( StatementOne, 
"update import set headrows = %d, colcount = %d, refnum = %d, date = %d, payee = %d, increase = %d, incsign = '%c', decrease = %d, decsign = '%c' where acctnum = %d",
			ximport.xheadrows,
			ximport.xcolcount,
			ximport.xrefnum,
			ximport.xdate,
			ximport.xpayee,
			ximport.xincrease,
			ximport.xincsign[0],
			ximport.xdecrease,
			ximport.xdecsign[0],
			AccountNumber );
	}
	else
	{
		sprintf ( StatementOne, 
			"update import set parsefunc = '%s' where acctnum = %d",
				ximport.xparsefunc,
				AccountNumber );
	}

// printf ( "%s<br>\n", StatementOne );
	rv = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
#endif

	return ( rv );
}
