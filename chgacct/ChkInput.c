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

#include	"chgacct.h"

int ChkInput ()
{
	int		rv = 0;

	if ( CurrentAcctnum < 1 || NewAcctnum < 1 )
	{
		SaveError ( "Account Numbers are required" );
		rv++;
	}

	if ( CurrentAcctnum > 0 && LoadOneAccount ( CurrentAcctnum ) != 1 )
	{
		SaveError ( "Invalid Current Account Number" );
		rv++;
	}
	else
	{
		sprintf ( CurrentName, "%s", xaccount.xacctname );
	}

	if ( NewAcctnum > 0 && LoadOneAccount ( NewAcctnum ) == 1 )
	{
		SaveError ( "New Account Number already exists" );
		sprintf ( NewName, "%s", xaccount.xacctname );
		rv++;
	}

	return ( rv );
}
