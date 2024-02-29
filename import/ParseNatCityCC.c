/*----------------------------------------------------------------------------
	Program : ParseNatCityCC.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Custom function for National City.
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

static	char	Payee[128];

int ParseNatCityCC ( char *Buffer, RESULT *Result )
{
	int		mx, yx, xi, xl;
	int		rv = 0;

//	printf ( "NatCityCC<br>\n" );

	tokcnt = GetTokensCSV ( Buffer, tokens, MAXTOKS, 0, 0 );
	if ( tokcnt < 5 )
	{
		return ( -1 );
	}

	xl = nsStrlen ( tokens[0] );
	mx = yx = 0;
	for ( xi = 0; xi < xl; xi++ )
	{
		if ( tokens[0][xi] == '/' )
		{
			tokens[0][xi] = '\0';
			if ( mx == 0 )
			{
				mx = xi + 1;
			}
			else
			{
				yx = xi + 1;
				break;
			}
		}
	}
	Result->Date.month = nsAtoi ( &tokens[0][0] );
	Result->Date.day   = nsAtoi ( &tokens[0][mx] );
	Result->Date.year4  = nsAtoi ( &tokens[0][yx] );

	Result->Amount = 0.0 - nsAtof ( tokens[3] );
	if ( Result->Amount > 0.0 )
	{
		Result->Payee = Payee;

		Result->Refnum = "CHARGE";
	}
	else
	{
		Result->Payee = "NATIONAL CITY";
		Result->Refnum = "PAYMENT";
	}

//	Result->Memo = tokens[4];

	return ( rv );
}
