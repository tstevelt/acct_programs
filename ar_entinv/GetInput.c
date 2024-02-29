/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_entinv.h"

int FindStore ( long LineNumber )
{
	int		ndx;

	for ( ndx = 0; ndx < StoreCount; ndx++ )
	{
		if ( StoreArray[ndx].LineNumber == LineNumber )
		{
			break;
		}
	}

	if ( ndx >= StoreCount )
	{
		StoreArray[StoreCount].LineNumber = LineNumber;
		ndx = StoreCount;
		StoreCount++;
	}

	return ( ndx );
}

void StoreDescr ( long LineNumber, char *Descr )
{
	int		ndx;

	ndx = FindStore ( LineNumber );

	StoreArray[ndx].Descr = Descr;
}

void StoreAmount ( long LineNumber, double Amount )
{
	int		ndx;

	ndx = FindStore ( LineNumber );

	StoreArray[ndx].Amount = (long)(0.5 + Amount * 100.0);
}

void GetInput ()
{
	int		xa;
	long	LineNumber;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "InvoiceNumber" ) == 0 )
		{
			xarinvh.xid = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "CustomerNumber" ) == 0 )
		{
			xarinvh.xcustomer = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "InvoiceDate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xarinvh.xinvdate );
		}
		else if ( nsStrcmp ( webNames[xa], "InvoiceTerms" ) == 0 )
		{
			snprintf ( xarinvh.xterms, sizeof(xarinvh.xterms), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "InvoiceStatus" ) == 0 )
		{
			xarinvh.xstatus[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "InvoicePoNum" ) == 0 )
		{
			snprintf ( xarinvh.xponum, sizeof(xarinvh.xponum), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "inserted" ) == 0 )
		{
			xarinvh.xinserted = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "updated" ) == 0 )
		{
			xarinvh.xupdated = nsAtol ( webValues[xa] );
		}

		else if ( nsStrncmp ( webNames[xa], "descr_", 6 ) == 0 )
		{
			LineNumber = nsAtol ( &webNames[xa][6] );
			StoreDescr ( LineNumber, webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "amount_", 7 ) == 0 )
		{
			LineNumber = nsAtol ( &webNames[xa][7] );
			StoreAmount ( LineNumber, nsAtof ( webValues[xa] ) );
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
		else if ( nsStrcmp ( webNames[xa], "submitAddHead" ) == 0 )
		{
			RunMode = MODE_ADD_HEAD;
		}
		else if ( nsStrcmp ( webNames[xa], "submitAddLine" ) == 0 )
		{
			RunMode = MODE_ADD_LINE;
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
