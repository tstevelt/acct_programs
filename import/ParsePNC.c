/*----------------------------------------------------------------------------
	Program : ParsePNC.c
	Author  : Tom Stevelt
	Date    : Once upon a time
	Synopsis: Construct REFNUM and PAYEE from Description.  Get dollar value
			  from string with dollar sign and comma ($1,234.56)
	Return  : 0 on success

	Who		Date		Modification
	---------------------------------------------------------------------
	tms		12/07/2020	Replaced acctlib/GetCSVtoks with shslib/GetTokensCSV
						Was going to use shslib/isword, but already working
						well enough with current logic.

----------------------------------------------------------------------------*/
#include	"import.h"

static	char	Payee[41];

#define		DEBUG_KROGER
#undef		DEBUG_KROGER

int ParsePNC ( char *Buffer, RESULT *Result )
{
	int		rv = 0;
	double	Withdrawal, Deposit;
	int		xl, xf, xi, xd;
	static	int		lineno = 0;

#ifdef DEBUG_KROGER
	rv = -1;
#endif

	lineno++;
	if ( nsStrncmp ( Buffer, "Date", 4 ) == 0 )
	{
		return ( -1 );
	}
	

	if ( Buffer[0] == '#' )
	{
		return ( -1 );
	}

#ifdef DEBUG_KROGER
printf ( "PNCcheck [%s]<br>\n", Buffer );
#endif

	tokcnt = GetTokensCSV ( Buffer, tokens, MAXTOKS, 0, 0 );
	if ( tokcnt < 4 )
	{
		return ( -1 );
	}

	Result->Date.year4  = nsAtoi ( &tokens[0][6] );
	Result->Date.year2  = Result->Date.year4 % 100;
	Result->Date.day   = nsAtoi ( &tokens[0][3] );
	Result->Date.month = nsAtoi ( &tokens[0][0] );

	tokcn2 = GetTokensW ( tokens[1], token2, MAXTOKS );

	if ( tokcn2 >= 2 && nsStrcmp ( token2[0], "CHECK" ) == 0 )
	{
		if ( nsStrcmp ( token2[1], "CARD" ) == 0 )
		{
			Result->Refnum = "DEBIT";
		}
		else
		{
			Result->Refnum = token2[1];
		}
	}
	else if ( nsStrcmp ( token2[0], "POS" ) == 0 )
	{
		Result->Refnum = "DEBIT";
	}
	else if ( nsStrcmp ( token2[0], "DEPOSIT" ) == 0 )
	{
		Result->Refnum = "DEPOSIT";
	}
	else if ( tokcn2 >= 4 && nsStrcmp ( token2[3], "FEE" ) == 0 )
	{
		Result->Refnum = "FEE";
	}
	else if ( tokcn2 >= 2 && nsStrcmp ( token2[0], "ATM" ) == 0 )
	{
		if ( nsStrcmp ( token2[1], "DEPOSIT" ) == 0 )
		{
			Result->Refnum = "DEPOSIT";
		}
		else
		{
			Result->Refnum = "ATM";
		}
	}
	else if ( nsStrcmp ( token2[0], "INTEREST" ) == 0 )
	{
		Result->Refnum = "INT";
	}
	else if ( tokcn2 >= 2 && nsStrcmp ( token2[0], "ACH" ) == 0 )
	{
		if ( nsStrcmp ( token2[1], "CREDIT" ) == 0 )
		{
			Result->Refnum = "DEPOSIT";
		}
		else
		{
			Result->Refnum = "ACH";
		}
	}
	else
	{
		Result->Refnum = token2[0];
	}

	/*--------------------------------------------------------------
		first pass, look for X's, set payee to everything after.
	--------------------------------------------------------------*/
	memset ( Payee, 0, sizeof(Payee) );
	for ( xl = 0, xf = 0; xl < tokcn2; xl++ )
	{
		if ( xl + 1 < tokcn2 && nsStrstr ( token2[xl], "XXX" ) != (char *)0 )
		{
			xf = 1;
		}
		else if ( xf > 0 )
		{
			if ( nsStrlen ( Payee ) + 1 + nsStrlen ( token2[xl] ) > 40 )
			{
				break;
			}

			if ( xf > 1 )
			{
				strcat ( Payee, " " );
			}
			strcat ( Payee, token2[xl] );
			xf++;
		}
	}

	for ( xl = 0; xf == 0 && xl < tokcn2; xl++ )
	{
		/*----------------------------------------------------------
			skip special words
		----------------------------------------------------------*/
		if ( nsStrcmp ( token2[xl], "POS" ) == 0 )
		{
			continue;
		}
		else if ( nsStrcmp ( token2[xl], "PURCHASE" ) == 0 )
		{
			continue;
		}

		/*----------------------------------------------------------
			skip words with digits in them
		----------------------------------------------------------*/
		for ( xi = 0, xd = 0; token2[xl][xi]; xi++ )
		{
			if ( isdigit ( token2[xl][xi] ) != 0 )
			{
				xd = 1;
				break;
			}
		}
		if ( xd )
		{
			continue;
		}

		/*----------------------------------------------------------
			found a normal word, keep rest of line.
		----------------------------------------------------------*/
		for ( xf = 0; xl < tokcn2; xl++ )
		{
			if ( nsStrlen ( Payee ) + 1 + nsStrlen ( token2[xl] ) > 40 )
			{
				break;
			}

			if ( xf > 0 )
			{
				strcat ( Payee, " " );
			}
			strcat ( Payee, token2[xl] );
			xf++;
		}
		break;
	}

	if ( xf == 0 )
	{
		/*--------------------------------------------------------------
			if still nothing, set to "payee"
		--------------------------------------------------------------*/
		nsStrcpy ( Payee, "payee" );
	}

#ifdef DEBUG_KROGER
printf ( "%s<br>\n", Payee );
#endif
	Result->Payee = Payee;

	tokens[2]  = FixMoney ( tokens[2] );
	Withdrawal = nsAtof ( tokens[2] );

	tokens[3] = FixMoney ( tokens[3] );
	Deposit   = nsAtof ( tokens[3] );

	if ( Withdrawal != 0.0 && Deposit == 0.0 )
	{
		Result->Amount = 0.0 - Withdrawal;
	}
	else if ( Withdrawal == 0.0 && Deposit != 0.0 )
	{
		Result->Amount = Deposit;
	}
	else
	{
		Result->Amount = 0.0;
		rv = -1;
	}

	return ( rv );
}
