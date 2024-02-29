/*----------------------------------------------------------------------------
	Program : FixMoney.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Ignore $ , etc.
	Return  : 
----------------------------------------------------------------------------*/

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
