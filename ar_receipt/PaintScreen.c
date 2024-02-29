/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
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

#include	"ar_receipt.h"

static int DebugPaintScreen = 0;

// static long InvoiceTotal = 0;
// 	InvoiceTotal += ptrArinvh->xamount;

static int EachInvoice ( XARINVH *ptrArinvh )
{
	long	InvoiceAmount;
	DBY_QUERY	*MyQuery;

	sprintf ( StatementTwo,
		"select sum(amount) from arinvl, arinvh where arinvl.invoice = arinvh.id and arinvh.id = %ld", ptrArinvh->xid );

	if ( DebugPaintScreen )
	{
		printf ( "%s<br>\n", StatementTwo );
	}

	MyQuery = dbySelect ( "ar_receipt", &MySql, StatementTwo, LOGFILENAME );

	MyQuery->EachRow = mysql_fetch_row ( MyQuery->Result );
	InvoiceAmount = nsAtol ( MyQuery->EachRow[0] );

	printf ( "<tr>\n" );

	printf ( "<td align='center'>%ld</td>\n", ptrArinvh->xid );
	printf ( "<td align='center'>%02d/%02d/%02d</td>\n", 
		ptrArinvh->xinvdate.month, ptrArinvh->xinvdate.day, ptrArinvh->xinvdate.year2 );

	printf ( "<td>%s</td>\n", ptrArinvh->xponum );

	printf ( "<td align='right'>%.2f</td>\n", (double) InvoiceAmount / 100.0 );
	printf ( "<input type='hidden' name='total_%ld' value='%.2f'>\n", ptrArinvh->xid, (double) InvoiceAmount / 100.0 );

	printf ( "<td align='right'>" );
	printf ( "<input type='search' name='discount_%ld' value='%.2f'>\n", ptrArinvh->xid, (double) ptrArinvh->xdiscount / 100.0 );
	printf ( "</td>\n" );

	printf ( "<td align='right'>" );
	printf ( "<input type='search' name='payment_%ld' value='%.2f'>\n", ptrArinvh->xid, (double) ptrArinvh->xpayment / 100.0 );
	printf ( "</td>\n" );

	printf ( "<td align='right'>" );
	printf ( "%.2f\n", (double) ( InvoiceAmount - ptrArinvh->xpayment - ptrArinvh->xdiscount ) / 100.0 );
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	return ( 0 );
}

void PaintScreen ()
{
	printf ( "<table class='AppWide'0%%'>\n" );

	/*----------------------------------------------------------
		row - customer number
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>customer number</td>\n" );
	printf ( "<td>\n" );
	if ( xarcust.xid == 0 )
	{
		printf ( "<input type='search' name='CustomerNumber'>\n" );
	}
	else
	{
		printf ( "%ld\n", xarcust.xid );
		printf ( "<input type='hidden' name='CustomerNumber' value='%ld'>\n", xarcust.xid );
	}
	printf ( "</td>\n" );
	printf ( "<td colspan='5'>%s</td>\n", xarcust.xname );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row - check number date amount
	----------------------------------------------------------*/
	printf ( "<input type='hidden' name='CheckNumber' value='%s'>\n", CheckNumber );

	printf ( "<input type='hidden' name='CheckDate' value='%04d-%02d-%02d'>\n",
				CheckDate.year4, CheckDate.month, CheckDate.day );

	printf ( "<input type='hidden' name='CheckAmount' value='%.2f'>\n", CheckAmount );

	printf ( "<tr>\n" );
	printf ( "<td>check number</td>\n" );
	printf ( "<td>\n" );
	if ( xarcust.xid == 0 )
	{
		printf ( "<input type='search' name='CheckNumber'>\n" );
	}
	else
	{
		printf ( "%s", CheckNumber );
	}
	printf ( "</td>\n" );
	printf ( "<td>date</td>\n" );
	printf ( "<td>\n" );
	if ( xarcust.xid == 0 )
	{
		printf ( "<input type='search' name='CheckDate'>\n" );
	}
	else
	{
		printf ( "%02d/%02d/%02d", CheckDate.month, CheckDate.day, CheckDate.year2 );
	}
	printf ( "</td>\n" );
	printf ( "<td>amount</td>\n" );
	printf ( "<td>\n" );
	if ( xarcust.xid == 0 )
	{
		printf ( "<input type='search' name='CheckAmount'>\n" );
	}
	else
	{
		printf ( "%.2f", CheckAmount );
	}
	printf ( "</td>\n" );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		multiple rows - open invoices
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='center'>invoice</td>\n" );
	printf ( "<td align='center'>date</td>\n" );
	printf ( "<td>po number</td>\n" );
	printf ( "<td align='right'>amount</td>\n" );
	printf ( "<td align='right'>discount</td>\n" );
	printf ( "<td align='right'>payment</td>\n" );
	printf ( "<td align='right'>balance</td>\n" );
	printf ( "</tr>\n" );

	if ( xarcust.xid )
	{
		snprintf ( WhereClause, sizeof(WhereClause), "arinvh.customer = %ld and status = 'O'", xarcust.xid );
		LoadArinvhCB ( WhereClause, "arinvh.invdate", &xarinvh, (int(*)()) EachInvoice, 0 );
	}

	/*----------------------------------------------------------
		bottom row: save button
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='7'>\n" );

	printf ( "<input type='submit' name='submitClear' value='clear'>\n" );

	if ( xarcust.xid )
	{
		if ( CheckAmount > 0.0 )
		{
			printf ( "&emsp;<input type='submit' name='submitSave' value='save'>\n" );
		}
	}
	else
	{
		printf ( "&emsp;<input type='submit' name='submitFind' value='find open invoices'>\n" );
	}
	
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
