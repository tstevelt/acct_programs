/*----------------------------------------------------------------------------
	Program : AutoParse.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Parse one line from the uploaded file.
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

#include	"import.h"

static int DebugAutoParse = 0;

int AutoParse ( char *Buffer, RESULT *Result )
{
	static	int		lineno = 0;
			int		rv = 0;
			double	Decrease, Increase;
			int		xt;

	lineno++;
	if ( lineno <= MyImport.xheadrows )
	{
		return ( -1 );
	}

	tokcnt = GetTokensCSV ( Buffer, tokens, MAXTOKS, 0, Verbose > 1 ? 1 : 0 );
	if ( tokcnt < MyImport.xcolcount )
	{
		if ( DebugAutoParse )
		{
			printf ( "tokcnt %d<br>\n", tokcnt );
		}
		return ( -1 );
	}

	if ( MyImport.xrefnum > 0 )
	{
		xt = MyImport.xrefnum - 1;
//TrimRightAndLeft ( tokens[xt] );
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		Result->Refnum = tokens[xt];
	}
	else
	{
		Result->Refnum = "";
	}

	if ( MyImport.xdate > 0 )
	{
		xt = MyImport.xdate - 1;
//TrimRightAndLeft ( tokens[xt] );
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		StringToDateval ( tokens[xt], &Result->Date );
	}
	else
	{
		Result->Date.year2  = 0;
		Result->Date.year4  = 0;
		Result->Date.day   = 0;
		Result->Date.month = 0;
	}

	if ( MyImport.xpayee > 0 )
	{
		xt = MyImport.xpayee - 1;
//TrimRightAndLeft ( tokens[xt] );
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		if ( DebugAutoParse )
		{
			printf ( "%d %s<br>\n", xt, tokens[xt] );
		}
		Result->Payee = tokens[xt];
	}
	else
	{
		Result->Payee = "";
	}

	Increase = 0.0;
	if ( MyImport.xincrease > 0 )
	{
		xt = MyImport.xincrease - 1;
//TrimRightAndLeft ( tokens[xt] );
		tokens[xt] = FixMoney ( tokens[xt] );
		Increase   = nsAtof ( tokens[xt] );

		if ( MyImport.xincsign[0] == 'Y' )
		{
			Increase = 0.0 - Increase;
		}
	}

	Decrease = 0.0;
	if ( MyImport.xdecrease > 0 )
	{
		xt = MyImport.xdecrease - 1;
//TrimRightAndLeft ( tokens[xt] );
		tokens[xt] = FixMoney ( tokens[xt] );
		Decrease   = nsAtof ( tokens[xt] );

		if ( MyImport.xdecsign[0] == 'Y' )
		{
			Decrease = 0.0 - Decrease;
		}
	}

	if ( Increase != 0.0 && Decrease == 0.0 )
	{
		Result->Amount = Increase;
		if ( Result->Refnum == (char *)0 || Result->Refnum[0] == '\0' )
		{
			switch ( MyAccount.xaccttype[0] )
			{
				case 'A': Result->Refnum = "DEPOSIT"; break;
				case 'L': Result->Refnum = "CHARGE"; break;
			}
		}
	}
	else if ( Increase == 0.0 && Decrease != 0.0 )
	{
		Result->Amount = Decrease;
		if ( Result->Refnum == (char *)0 || Result->Refnum[0] == '\0' )
		{
			switch ( MyAccount.xaccttype[0] )
			{
				case 'A': Result->Refnum = "WITHDRAW"; break;
				case 'L': Result->Refnum = "PAYMENT"; break;
			}
		}
	}
	else
	{
		Result->Amount = 0.0;
		rv = -1;
	}

	return ( rv );
}
