/*----------------------------------------------------------------------------
	Program : Find.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint invoice lookup invoices
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

#include	"ar_entinv.h"

static int DebugFind = 0;

static XARINVH LookupArinvh;

static int MyEachArinvh ( XARINVH *ptrArinvh )
{
	static int firstpass = 1;

	if ( firstpass )
	{
		firstpass = 0;

		printf ( "<table class='AppWide'>\n" );
		printf ( "<tr>\n" );

		printf ( "<td>id</td>\n" );
		printf ( "<td>customer</td>\n" );
		printf ( "<td>invdate</td>\n" );
		printf ( "<td>terms</td>\n" );
		printf ( "<td>status</td>\n" );
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>" );
	printf ( "<a href='ar_entinv.cgi?submitFind=load&InvoiceNumber=%ld'>%ld</a>", ptrArinvh->xid, ptrArinvh->xid );
	printf ( "</td>\n" );
	printf ( "<td>%ld</td>\n", ptrArinvh->xcustomer );
	printf ( "<td>%02d/%02d/%02d</td>\n", ptrArinvh->xinvdate.month, ptrArinvh->xinvdate.day, ptrArinvh->xinvdate.year2 );
	printf ( "<td>%s</td>\n", ptrArinvh->xterms );
	printf ( "<td>%s</td>\n", ptrArinvh->xstatus );
	printf ( "</tr>\n" );

	return ( 0 );
}

int Find ()
{
	int		rv;
	int		xo;
	char	Fragment[128];


	/*----------------------------------------------------------
		given an invoice number, just look it up
	----------------------------------------------------------*/
	if ( xarinvh.xid > 0 )
	{
		snprintf ( WhereClause, sizeof(WhereClause), "arinvh.id = %ld", xarinvh.xid );

		if ( LoadArinvh ( WhereClause, &xarinvh, 0 ) != 1 )
		{
			SaveError ( "Invoice Number not found" );
			xarinvh.xid = 0;
			return ( 0 );
		}

		snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
		if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
		{
			SaveError ( "Customer not found" );
			return ( 0 );
		}

		return ( 1 );
	}

	memset ( WhereClause, '\0', sizeof(WhereClause) );
	xo = 0;

	if ( xarinvh.xcustomer > 0 )
	{
		sprintf ( Fragment, "customer = %ld", xarinvh.xcustomer );

		if ( xo )
		{
			nsStrcat ( WhereClause, " and " );
		}
		nsStrcat ( WhereClause, Fragment );
		xo++;
	}

	if ( xarinvh.xinvdate.month > 0 )
	{
		sprintf ( Fragment, "invdate = '%04d-%02d-%02d'", 
			xarinvh.xinvdate.year4,
			xarinvh.xinvdate.month, xarinvh.xinvdate.day );

		if ( xo )
		{
			nsStrcat ( WhereClause, " and " );
		}
		nsStrcat ( WhereClause, Fragment );
		xo++;
	}

	if ( lastchr ( xarinvh.xterms, sizeof(xarinvh.xterms) ) > 0 )
	{
		if ( dbyWildCards ( xarinvh.xterms ) > 0 )
		{
			sprintf ( Fragment, "terms like '%s'", xarinvh.xterms );
		}
		else
		{
			sprintf ( Fragment, "terms = '%s'", xarinvh.xterms );
		}

		if ( xo )
		{
			nsStrcat ( WhereClause, " and " );
		}
		nsStrcat ( WhereClause, Fragment );
		xo++;
	}

	if ( lastchr ( xarinvh.xponum, sizeof(xarinvh.xponum) ) > 0 )
	{
		if ( dbyWildCards ( xarinvh.xponum ) > 0 )
		{
			sprintf ( Fragment, "ponum like '%s'", xarinvh.xponum );
		}
		else
		{
			sprintf ( Fragment, "ponum = '%s'", xarinvh.xponum );
		}

		if ( xo )
		{
			nsStrcat ( WhereClause, " and " );
		}
		nsStrcat ( WhereClause, Fragment );
		xo++;
	}

	if ( xo == 0 )
	{
		SaveError ( "Please enter something to search for." );
		SaveError ( "Wildcards (asterisk) are allowed." );
		RunMode = MODE_START;
		return ( 0 );
	}

	if ( DebugFind )
	{
		printf (  "%s<br>\n", WhereClause );
	}

	rv = LoadArinvhCB ( WhereClause, "invdate desc", &LookupArinvh, (int(*)()) MyEachArinvh, 1 );

    if ( rv < 0 )
    {
        SaveError ( "Oops!" );
    }
    else if ( rv == 0 )
    {
        SaveError ( "Nothing matches search criteria entered." );
    }
	
	return ( rv );
}
