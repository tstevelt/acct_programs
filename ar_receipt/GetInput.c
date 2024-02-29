/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_receipt.h"

int FindStore ( long InvoiceNumber )
{
	int		ndx;

	for ( ndx = 0; ndx < StoreCount; ndx++ )
	{
		if ( StoreArray[ndx].InvoiceNumber == InvoiceNumber )
		{
			break;
		}
	}

	if ( ndx >= StoreCount )
	{
		StoreArray[StoreCount].InvoiceNumber = InvoiceNumber;
		ndx = StoreCount;
		StoreCount++;
	}

	return ( ndx );
}

void StoreTotal ( long InvoiceNumber, double Total )
{
	int		ndx;

	ndx = FindStore ( InvoiceNumber );

	StoreArray[ndx].Total = (long)(0.5 + Total * 100.0);
}

void StoreDiscount ( long InvoiceNumber, double Discount )
{
	int		ndx;

	ndx = FindStore ( InvoiceNumber );

	StoreArray[ndx].Discount = (long)(0.5 + Discount * 100.0);
}

void StorePayment ( long InvoiceNumber, double Payment )
{
	int		ndx;

	ndx = FindStore ( InvoiceNumber );

	StoreArray[ndx].Payment = (long)(0.5 + Payment * 100.0);
}

void GetInput ()
{
	int		xa;
	long	InvoiceNumber;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "CustomerNumber" ) == 0 )
		{
			xarcust.xid = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CheckNumber" ) == 0 )
		{
			snprintf ( CheckNumber, sizeof(CheckNumber), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CheckDate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &CheckDate );
		}
		else if ( nsStrcmp ( webNames[xa], "CheckAmount" ) == 0 )
		{
			CheckAmount = nsAtof ( webValues[xa] );
		}

		else if ( nsStrncmp ( webNames[xa], "total_", 6 ) == 0 )
		{
			InvoiceNumber = nsAtol ( &webNames[xa][6] );
			StoreTotal ( InvoiceNumber, nsAtof ( webValues[xa] ) );
		}
		else if ( nsStrncmp ( webNames[xa], "discount_", 9 ) == 0 )
		{
			InvoiceNumber = nsAtol ( &webNames[xa][9] );
			StoreDiscount ( InvoiceNumber, nsAtof ( webValues[xa] ) );
		}
		else if ( nsStrncmp ( webNames[xa], "payment_", 8 ) == 0 )
		{
			InvoiceNumber = nsAtol ( &webNames[xa][8] );
			StorePayment ( InvoiceNumber, nsAtof ( webValues[xa] ) );
		}

		else if ( nsStrcmp ( webNames[xa], "submitClear" ) == 0 )
		{
			RunMode = MODE_START;
			memset ( &xarcust, '\0', sizeof(xarcust) );
			memset ( &xarinvh, '\0', sizeof(xarinvh) );
			memset ( &xarinvl, '\0', sizeof(xarinvl) );
		}
		else if ( nsStrcmp ( webNames[xa], "submitFind" ) == 0 )
		{
			RunMode = MODE_FIND;
		}
		else if ( nsStrcmp ( webNames[xa], "submitSave" ) == 0 )
		{
			RunMode = MODE_SAVE;
		}

		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}

	}
	
}
