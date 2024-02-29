/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Make invoice file (PDF)
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

#include	"ar_prtinv.h"

static	int		DebugReport = 0;
static	int		InvoiceWidth = 70;
static	int		LinesPerPage = 55;
static	int		lineno = 0;
static	int		pageno = 0;
static	long	InvoiceTotal = 0;

static void PrintHeader ()
{
	char	HeadLine[128];
	int		Indent, Length;

	pageno++;
	lineno = 0;
	
	if ( pageno > 1 )
	{
		fprintf ( fpData, "\nCONTINUED\n" );
		fprintf ( fpData, "\f" );
	}

	/*----------------------------------------------------------
		to center:
	----------------------------------------------------------*/
	if ( pageno == 1 )
	{
		sprintf ( HeadLine, "INVOICE" );
	}
	else
	{
		sprintf ( HeadLine, "INVOICE - PAGE %d", pageno );
	}
	Indent = (InvoiceWidth - nsStrlen ( HeadLine ) ) / 2;

	fprintf ( fpData, "\n\n" );
	lineno += 2;

	fprintf ( fpData, "%*.*s%s\n\n", Indent, Indent, " ", HeadLine );
	lineno += 2;

	Indent = 3;

	fprintf ( fpData, "%*.*s%s\n", Indent, Indent, " ", xsystem.xname );
	lineno++;

	fprintf ( fpData, "%*.*s%s\n", Indent, Indent, " ", xsystem.xaddress );
	lineno++;

	fprintf ( fpData, "%*.*s%s, %s %s\n", Indent, Indent, " ", xsystem.xcity, xsystem.xstate, xsystem.xzipcode );
	lineno++;

	fprintf ( fpData, "%*.*s%s\n", Indent, Indent, " ", xsystem.xphone );
	lineno++;

	fprintf ( fpData, "\n\n\n\n\n" );
	lineno += 5;

	Length = fprintf ( fpData, "%*.*s%s", Indent, Indent, " ", xarcust.xname );
	Length = 42 - Length;
	fprintf ( fpData, "%*.*sInvoice Number: %ld\n", Length, Length, " ", xarinvh.xid );
	lineno++;

	Length = fprintf ( fpData, "%*.*s%s", Indent, Indent, " ", xarcust.xaddr1 );
	Length = 42 - Length;
	fprintf ( fpData, "%*.*sPurchase Order: %s\n", Length, Length, " ", xarinvh.xponum );
	lineno++;

	if ( nsStrlen ( xarcust.xaddr2 ) > 0 )
	{
		Length = fprintf ( fpData, "%*.*s%s", Indent, Indent, " ", xarcust.xaddr2 );
		Length = 42 - Length;
		fprintf ( fpData, "%*.*sInvoice Date  : %d/%d/%d\n", Length, Length, " ", 
					xarinvh.xinvdate.month, xarinvh.xinvdate.day, xarinvh.xinvdate.year2 );
		lineno++;

		Length = fprintf ( fpData, "%*.*s%s, %s %s", Indent, Indent, " ", xarcust.xcity, xarcust.xstate, xarcust.xzipcode );
		Length = 42 - Length;
		fprintf ( fpData, "%*.*sPayment Terms : %s\n", Length, Length, " ", xarinvh.xterms );
		lineno++;

		Length = fprintf ( fpData, "%*.*sATTN: %s\n\n", Indent, Indent, " ", xarcust.xcontact );
		lineno += 2;
	}
	else
	{
		Length = fprintf ( fpData, "%*.*s%s, %s %s", Indent, Indent, " ", xarcust.xcity, xarcust.xstate, xarcust.xzipcode );
		Length = 42 - Length;
		fprintf ( fpData, "%*.*sInvoice Date  : %d/%d/%d\n", Length, Length, " ", 
					xarinvh.xinvdate.month, xarinvh.xinvdate.day, xarinvh.xinvdate.year2 );
		lineno++;

		Length = fprintf ( fpData, "%*.*sATTN: %s", Indent, Indent, " ", xarcust.xcontact );
		Length = 42 - Length;
		fprintf ( fpData, "%*.*sPayment Terms : %s\n\n", Length, Length, " ", xarinvh.xterms );
		lineno += 2;
	}

	fprintf ( fpData, "+------+-------------------------------------------------+-----------+\n" );
	lineno++;
	fprintf ( fpData, "| Line | Description                                     |    Amount |\n" );
	lineno++;
	fprintf ( fpData, "+------+-------------------------------------------------+-----------+\n" );
	lineno++;
}

static char *InstructionArray [] =
{
// 123456789-123456789-123456789-123456789-123456789-123456789-
	"Payment may be sent via USPS to address above.",
	"ACH Routing 083904563 Account 1173617901",
	"Paypal to: Silver Hammer Software LLC",
//	"Paypal QR: See attached QR code",
};

static int InstructionCount = sizeof(InstructionArray) / sizeof(char *);

static void PrintInstructions ()
{
	int		ndx;

	if ( lineno + InstructionCount > LinesPerPage )
	{
		PrintHeader ();
	}

	fprintf ( fpData, "   +----------------------------------------------------------+\n" );
	lineno++;
	for ( ndx = 0; ndx < InstructionCount; ndx++ )
	{
		fprintf ( fpData, "   | %-56.56s |\n", InstructionArray[ndx] );
		lineno++;
	}
	fprintf ( fpData, "   +----------------------------------------------------------+\n" );
	lineno++;
}

/*---------------------------------------------------------------------------
+------+-------------------------------------------------+-----------+
| Line | Description                                     |    Amount |
+------+-------------------------------------------------+-----------+
  1234   123456789-123456789-123456789-123456789-123456    123456789
typedef struct
{
	char	*Start;
	int		Length;
} RECORD;

#define	MAXLINES	10
static	RECORD		LineArray[MAXLINES];
---------------------------------------------------------------------------*/
static	int			ItemCount;
static	int			LineCount;
#define	RPTWIDTH	46

#define		MAXTOKS		10000
static	char	*tokens[MAXTOKS];
static	int		tokcnt;

static int EachInvoiceLine ( XARINVL *ptrArinvl )
{
	int		MaxTokens, dlen, tlen, xt;
	int		Spacer;
	int		EstimatedLines;

	EstimatedLines = 1 + nsStrlen ( ptrArinvl->xdescr ) / RPTWIDTH;
	
	if ( lineno + EstimatedLines > LinesPerPage )
	{
		PrintHeader ();
	}

	ItemCount++;

	MaxTokens = 3 + nsCharCount ( ptrArinvl->xdescr, ' ' );
	if ( MaxTokens >= MAXTOKS )
	{
		printf ( "EachInvoiceLine: Exceeds MAXTOKS<br>\n" );
		exit ( 0 );
	}

	tokcnt = GetTokensStd ( ptrArinvl->xdescr, " ", tokens, MAXTOKS, 0 );

	for ( LineCount = 0, xt = 0; xt < tokcnt; )
	{
		if ( LineCount == 0 )
		{
			fprintf ( fpData, "  %4d   ", ItemCount /* ptrArinvl->xid meaningless to user */ );
		}
		else
		{
			fprintf ( fpData, "%9.9s", " " );
		}

		for ( dlen = 0; xt < tokcnt; )
		{
			tlen = nsStrlen ( tokens[xt] );
			if ( dlen + tlen + 1 > RPTWIDTH )
			{
				break;
			}

			dlen += fprintf ( fpData, "%s%s", dlen == 0 ? "" : " ", tokens[xt++] );
		}

		if ( LineCount == 0 )
		{
			Spacer = 4 + RPTWIDTH - dlen;

			if ( Spacer > 0 )
			{
				fprintf ( fpData, "%*.*s", Spacer, Spacer, " " );
			}

			fprintf ( fpData, "%9.2f\n", (double) ptrArinvl->xamount / 100.0 );
		}
		else
		{
			fprintf ( fpData, "\n" );
		}
		lineno++;
		LineCount++;
	}


#ifdef OLD_STUVV
	int		TotalLength, WhitePos, LineStart, ndx;
	TotalLength = nsStrlen ( ptrArinvl->xdescr );
	LineCount = 0;
	LineStart = 0;
	LineArray[LineCount].Length = 0;
	for ( ndx = 0, WhitePos = 0; ndx < TotalLength; ndx++ )
	{
		if ( ptrArinvl->xdescr[ndx] == ' ' )
		{
			WhitePos = ndx;
		}
		LineArray[LineCount].Length++;
		if ( LineArray[LineCount].Length == RPTWIDTH )
		{
			ptrArinvl->xdescr[WhitePos] = '\0';

			LineArray[LineCount].Start = &ptrArinvl->xdescr[LineStart];
			LineArray[LineCount].Length = WhitePos - LineStart;

			LineCount++;
			LineArray[LineCount].Length = 0;
			LineStart = WhitePos + 1;
		}
	}

	LineArray[LineCount].Start = &ptrArinvl->xdescr[LineStart];
	LineArray[LineCount].Length = WhitePos - LineStart;
	LineCount++;

	for ( ndx = 0; ndx < LineCount; ndx++ )
	{
		if ( ndx == 0 )
		{
			fprintf ( fpData, "  %4ld   ", ptrArinvl->xid );
			fprintf ( fpData, "%-*.*s    ", RPTWIDTH, RPTWIDTH, LineArray[ndx].Start );
			fprintf ( fpData, "%9.2f\n", (double) ptrArinvl->xamount / 100.0 );
		}
		else
		{
			fprintf ( fpData, "%9.9s%-*.*s\n", " ", RPTWIDTH, RPTWIDTH, LineArray[ndx].Start );
		}
		lineno++;
	}
#endif

	InvoiceTotal += ptrArinvl->xamount;

	return ( 0 );
}

int getdata ()
{
	char	WhereClause[128];
	char	OrderByClause[128];
	int		Indent;
	long	PaymentAmount, DueAmount;

	sprintf ( fnData, "%s/invoice_%ld.txt", TEMPDIR, InvoiceNumber );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		SaveError ( fnData );
		return ( -1 );
	}

if ( DebugReport )
{
	printf ( "kilroy was here %s<br>\n", fnData );
}
	LoadSystem ();

	snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
	LoadArcust ( WhereClause, &xarcust, 0 );

	PrintHeader ();

	ItemCount = 0;
	snprintf ( WhereClause, sizeof(WhereClause), "arinvl.invoice = %ld", xarinvh.xid );
	sprintf ( OrderByClause, "arinvl.id" );
	LoadArinvlCB ( WhereClause, OrderByClause, &xarinvl, (int(*)()) EachInvoiceLine, 0 );

	if ( lineno + 6 > LinesPerPage )
	{
		PrintHeader ();
	}

	fprintf ( fpData, "\n\n" );
	lineno += 2;

	Indent = InvoiceWidth - 9 - 7 - 2;
	fprintf ( fpData, "%*.*sTOTAL: %9.2f\n", Indent, Indent, " ", (double) InvoiceTotal / 100.0 );
	lineno++;

	PaymentAmount = xarinvh.xpayment + xarinvh.xdiscount;
	DueAmount = InvoiceTotal - PaymentAmount;

	if ( PaymentAmount > 0 )
	{
		int		CheckNumberLength;

		CheckNumberLength = nsStrlen ( xarinvh.xrefnum );

		Indent = InvoiceWidth - 9 - 7 - CheckNumberLength - 2;
		fprintf ( fpData, "%*.*sPAID %s: %9.2f\n", Indent, Indent, " ", xarinvh.xrefnum, (double) xarinvh.xpayment / 100.0 );
		lineno++;

		if ( xarinvh.xdiscount > 0 )
		{
			Indent = InvoiceWidth - 9 - 10 - 2;
			fprintf ( fpData, "%*.*sDISCOUNT: %9.2f\n", Indent, Indent, " ", (double) xarinvh.xdiscount / 100.0 );
			lineno++;
		}

		Indent = InvoiceWidth - 9 - 5 - 2;
		fprintf ( fpData, "%*.*sDUE: %9.2f\n", Indent, Indent, " ", (double) DueAmount / 100.0 );
		lineno++;
	}

	fprintf ( fpData, "\n\n" );
	lineno += 2;

	
	if ( IncludeInstructions == 'Y' )
	{
		PrintInstructions ();
	}

	for ( ; lineno < LinesPerPage; lineno++ )
	{
		fprintf ( fpData, "\n" );
	}

	Indent = (InvoiceWidth - nsStrlen ( "THANK YOU!" ) ) / 2;
	fprintf ( fpData, "%*.*sTHANK YOU!\n", Indent, Indent, " " );

	nsFclose ( fpData );
	return ( lineno );
}

void dorpt ()
{
	char	cmdline[1024];
	char	BaseName[200];
	char	*cp;

	sprintf ( BaseName, fnData );

	if (( cp = strrchr ( BaseName, '.' )) != (char *)0 )
	{
		*cp = '\0';
	}
	
	sprintf ( fnPS, "%s.ps", BaseName );
	sprintf ( fnPDF, "%s.pdf", BaseName );
	sprintf ( fnHtml, "%s.html", BaseName );

	sprintf ( cmdline, "/usr/local/bin/txt2ps %s -of %s -ff -tm 0.0 -bm 0.0", fnData, fnPS );
	system ( cmdline );

	sprintf ( cmdline, "/usr/bin/ps2pdf %s %s", fnPS, fnPDF );
	system ( cmdline );

//	sprintf ( cmdline, "/usr/local/bin/txt2html %s > %s", fnData, fnHtml );
//	system ( cmdline );

}
