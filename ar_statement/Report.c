/*----------------------------------------------------------------------------
	Program : Report.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Create statement file (PDF)
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

#include	"ar_statement.h"

static	int		DebugReport = 0;
static	int		InvoiceWidth = 70;
static	int		LinesPerPage = 55;
static	int		lineno = 0;
static	int		pageno = 0;
static	long	InvoiceTotal = 0;
static	long	TotalSales = 0;
static	long	TotalDiscount = 0;
static	long	TotalPaid = 0;
static	long	TotalDue = 0;

typedef struct
{
	int		From;
	int		To;
	long	Total;
} RECORD;

#define		MAXAGE		999

static	RECORD	DaysArray [] =
{
	{ 0,  30, 0 },
	{ 31, 60, 0 },
	{ 61, 90, 0 },
	{ 90, MAXAGE, 0 },
};

static	int		DaysCount = sizeof(DaysArray) / sizeof(RECORD);

static void StoreDaysTotal ( DATEVAL dvInvoice, long InvoiceTotal )
{
	int		InvoiceAge, ndx;

	InvoiceAge = DateDiff ( &dvInvoice, &dvToday );
	if ( DebugReport )
	{
		printf ( "Invoice Age %d days<br>\n", InvoiceAge );
	}

	/*----------------------------
		future dated
	----------------------------*/
	if ( InvoiceAge < 0 )
	{
		InvoiceAge = 0;
	}

	if ( InvoiceAge >= MAXAGE )
	{
		InvoiceAge = MAXAGE - 1;
	}

	for ( ndx = 0; ndx < DaysCount; ndx++ )
	{
		if ( DaysArray[ndx].From <= InvoiceAge && DaysArray[ndx].To >= InvoiceAge )
		{
			DaysArray[ndx].Total += InvoiceTotal;
			break;
		}
	}
}

static void PrintHeader ()
{
	int			Indent, Length;

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
	Indent = (InvoiceWidth - nsStrlen ( "STATEMENT" ) ) / 2;

	fprintf ( fpData, "\n\n" );
	lineno += 2;

	fprintf ( fpData, "%*.*s%s\n\n", Indent, Indent, " ", "STATEMENT" );
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
	Length = 52 - Length;
	fprintf ( fpData, "%*.*sStatement Date: %02d/%02d/%02d\n", Length, Length, " ", dvToday.month, dvToday.day, dvToday.year2 );
	lineno++;

	Length = fprintf ( fpData, "%*.*s%s\n", Indent, Indent, " ", xarcust.xaddr1 );
	lineno++;

	if ( nsStrlen ( xarcust.xaddr2 ) > 0 )
	{
		Length = fprintf ( fpData, "%*.*s%s\n", Indent, Indent, " ", xarcust.xaddr2 );
		lineno++;
	}

	Length = fprintf ( fpData, "%*.*s%s, %s %s\n", Indent, Indent, " ", xarcust.xcity, xarcust.xstate, xarcust.xzipcode );
	lineno++;

	Length = fprintf ( fpData, "%*.*sATTN: %s\n\n", Indent, Indent, " ", xarcust.xcontact );
	lineno += 2;

//                     ! 123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456 !
//                       99999999 mm/dd/yy x20xxxxxxxxxxxxxxxxx 99999.99 99999.99 99999.99 99999.99
	fprintf ( fpData, "+----------------------------------------------------------------------------+\n" );
	lineno++;
	fprintf ( fpData, "| Invoice  Date     PO Number              Amount     Disc     Paid      Due |\n" );
	lineno++;
	fprintf ( fpData, "+----------------------------------------------------------------------------+\n" );
	lineno++;
}

static int EachInvoice ( XARINVH *ptrArinvh )
{
	long	AmountDue;

	snprintf ( WhereClause, sizeof(WhereClause), "invoice = %ld", ptrArinvh->xid );
	InvoiceTotal = dbySelectSumLong ( &MySql, "arinvl", "amount", WhereClause, LOGFILENAME );
	AmountDue = InvoiceTotal - ptrArinvh->xdiscount - ptrArinvh->xpayment;

	TotalSales    += InvoiceTotal;
	TotalDiscount += ptrArinvh->xdiscount;
	TotalPaid     +=  ptrArinvh->xpayment;
	TotalDue      += AmountDue;

	fprintf ( fpData, " %8ld", ptrArinvh->xid );
	fprintf ( fpData, "  %02d/%02d/%02d", ptrArinvh->xinvdate.month, ptrArinvh->xinvdate.day, ptrArinvh->xinvdate.year2 );
	fprintf ( fpData, " %-20.20s", ptrArinvh->xponum );
	fprintf ( fpData, " %8.2f", (double) InvoiceTotal / 100.0 );
	fprintf ( fpData, " %8.2f", (double) ptrArinvh->xdiscount / 100.0 );
	fprintf ( fpData, " %8.2f", (double) ptrArinvh->xpayment / 100.0 );
	fprintf ( fpData, " %8.2f", (double) AmountDue / 100.0 );

	fprintf ( fpData, "\n" );
	lineno++;
		
	StoreDaysTotal ( ptrArinvh->xinvdate, AmountDue );

	return ( 0 );
}

int getdata ()
{
	char		WhereClause[128];
	char		OrderByClause[128];
	int			Indent;

	sprintf ( fnData, "%s/statement_%ld.txt", TEMPDIR, CustomerNumber );
	if (( fpData = fopen ( fnData, "w" )) == (FILE *) 0 )
	{
		SaveError ( "can not create report file" );
		return ( -1 );
	}

	CurrentDateval ( &dvToday );

if ( DebugReport )
{
	printf ( "kilroy was here %s<br>\n", fnData );
}
	LoadSystem ();

	snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", CustomerNumber );
	LoadArcust ( WhereClause, &xarcust, 0 );

	PrintHeader ();

	snprintf ( WhereClause, sizeof(WhereClause), "arinvh.customer = %ld", CustomerNumber );
	sprintf ( OrderByClause, "arinvh.invdate" );
	LoadArinvhCB ( WhereClause, OrderByClause, &xarinvh, (int(*)()) EachInvoice, 0 );

	if ( lineno + 3 > LinesPerPage )
	{
		PrintHeader ();
	}

	Indent = 40;
	fprintf ( fpData, "%*.*s ======== ======== ======== ========\n", Indent, Indent, " " );
	lineno++;

	Indent = 33;
	fprintf ( fpData, "%*.*sTOTALS:", Indent, Indent, " " );
	fprintf ( fpData, " %8.2f", (double) TotalSales / 100.0 );
	fprintf ( fpData, " %8.2f", (double) TotalDiscount / 100.0 );
	fprintf ( fpData, " %8.2f", (double) TotalPaid / 100.0 );
	fprintf ( fpData, " %8.2f", (double) TotalDue / 100.0 );
	fprintf ( fpData, "\n\n\n\n" );
	lineno += 4;
	
	if ( lineno + 5 > LinesPerPage )
	{
		PrintHeader ();
	}
	else
	{
		for ( ; lineno < LinesPerPage - 5; lineno++ )
		{
			fprintf ( fpData, "\n" );
		}
	}

	fprintf ( fpData, "+------------+------------+------------+------------+\n" );
	fprintf ( fpData, "|   Current  |  %d to %d  |  %d to %d  |   Over %d  |\n",
				DaysArray[1].From, DaysArray[1].To, DaysArray[2].From, DaysArray[2].To, DaysArray[3].From );
	fprintf ( fpData, "+------------+------------+------------+------------+\n" );
	fprintf ( fpData, "|  %8.2f  |  %8.2f  |  %8.2f  |  %8.2f  |\n",
				(double) DaysArray[0].Total / 100.0, 
				(double) DaysArray[1].Total / 100.0, 
				(double) DaysArray[2].Total / 100.0, 
				(double) DaysArray[3].Total / 100.0 );			
	fprintf ( fpData, "+------------+------------+------------+------------+\n" );

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

	sprintf ( cmdline, "/usr/local/bin/txt2ps %s -of %s -tm 0.0 -bm 0.0", fnData, fnPS );
	system ( cmdline );

	sprintf ( cmdline, "/usr/bin/ps2pdf %s %s", fnPS, fnPDF );
	system ( cmdline );

//	sprintf ( cmdline, "/usr/local/bin/txt2html %s > %s", fnData, fnHtml );
//	system ( cmdline );

}
