/*----------------------------------------------------------------------------
	Program : FixMoney.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Ignore $ , etc.
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

static	char	MoneyBuffer[30];

char *FixMoney ( char *Token )
{
	int		xl, xi, xo;

	memset ( MoneyBuffer, 0, sizeof(MoneyBuffer) );
	xl = nsStrlen ( Token );

	for ( xi = 0, xo = 0; xi < xl && xo < sizeof(MoneyBuffer); xi++ )
	{
		if ( Token[xi] >= '0' && Token[xi] <= '9' )
		{
			MoneyBuffer[xo++] = Token[xi];
		}
		else if ( Token[xi] == '-'  )
		{
			MoneyBuffer[xo++] = Token[xi];
		}
		else if ( Token[xi] == '.'  )
		{
			MoneyBuffer[xo++] = Token[xi];
		}
	}

	return ( MoneyBuffer );
}
