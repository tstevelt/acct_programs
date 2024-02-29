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

#include	"asetup.h"

int ChkInput ()
{
	int		rv = 0;
	
	if ( xsystem.xclosemethod[0] == 'M' )
	{
		xsystem.xretainequity = 0;
		xsystem.xretainincome = 0;
		return ( rv );
	}

	if ( xsystem.xretainequity == 0 )
	{
		SaveError ( "Retained Earnings Equity Account is REQUIRED" );
		rv++;
	}
	else
	{
		if ( LoadOneAccount ( xsystem.xretainequity ) == 0 )
		{
			SaveError ( "Retained Earnings Equity Account NOT FOUND" );
			xsystem.xretainequity = 0;
			rv++;
		}
		else if ( xaccount.xaccttype[0] != 'E' )
		{
			SaveError ( "Retained Earnings Equity Account is not an EQUITY ACCOUNT" );
			xsystem.xretainequity = 0;
			rv++;
		}
	}

	if ( xsystem.xretainincome == 0 )
	{
		SaveError ( "Retained Earnings Income Contra Account is REQUIRED" );
		rv++;
	}
	else
	{
		if ( LoadOneAccount ( xsystem.xretainincome ) == 0 )
		{
			SaveError ( "Retained Earnings Income Contra Account NOT FOUND" );
			xsystem.xretainincome = 0;
			rv++;
		}
		else if ( xaccount.xaccttype[0] != 'I' )
		{
			SaveError ( "Retained Earnings Income Contra Account is not an INCOME ACCOUNT" );
			xsystem.xretainincome = 0;
			rv++;
		}
	}

	if (( xsystem.xretainequity > 0  &&  xsystem.xretainincome == 0 ) ||
		( xsystem.xretainequity == 0  &&  xsystem.xretainincome > 0 ))
	{
		SaveError ( "Both accounts are required" );
		xsystem.xretainincome = 0;
		rv++;
	}

	return ( rv );
}
