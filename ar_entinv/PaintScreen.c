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

#include	"ar_entinv.h"

static long InvoiceTotal = 0;

static int EachLine ( XARINVL *ptrArinvl )
{
	int		Rows = 2;

	printf ( "<tr>\n" );

	printf ( "<td valign='top' align='center'>%ld</td>\n", ptrArinvl->xid );
	// printf ( "<td class='w3-top-align'>%ld</td>\n", ptrArinvl->xid );
	// printf ( "<td class='w3-display-topmiddle'>%ld</td>\n", ptrArinvl->xid );

	printf ( "<td colspan='2'>" );

	Rows = 1 + nsStrlen ( ptrArinvl->xdescr ) / 50;
	if ( Rows < 2 )
	{
		Rows = 2;
	}
	printf ( "<textarea name='descr_%ld' cols='60' rows='%d' maxlength='512'>%s</textarea>\n", 
								ptrArinvl->xid, Rows, ptrArinvl->xdescr );
	printf ( "</td>\n" );

	printf ( "<td valign='top' align='right'>" );
	printf ( "<input type='search' name='amount_%ld' value='%.2f'>\n", ptrArinvl->xid, (double) ptrArinvl->xamount / 100.0 );
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	InvoiceTotal += ptrArinvl->xamount;

	return ( 0 );
}

void PaintScreen ()
{
	printf ( "<table class='AppWide'0%%'>\n" );

	/*----------------------------------------------------------
		row - invoice number
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>invoice number</td>\n" );
	printf ( "<td>\n" );
	if ( xarinvh.xid == 0 )
	{
		printf ( "<input type='search' name='InvoiceNumber'>\n" );
	}
	else
	{
		printf ( "<input type='hidden' name='InvoiceNumber' value='%ld'>\n", xarinvh.xid );
		printf ( "%ld", xarinvh.xid );
	}
	printf ( "</td>\n" );
	printf ( "<td colspan='2'>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row - customer number
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>customer number</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='CustomerNumber'" );
	if ( xarcust.xid > 0 )
	{
		printf ( " value='%ld'", xarcust.xid );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "<td colspan='2'>%s</td>\n", xarcust.xname );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row - invoice date and invoice terms
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>date</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='InvoiceDate'" );
	if ( xarinvh.xinvdate.month > 0 )
	{
		printf ( " value='%02d/%02d/%02d'", xarinvh.xinvdate.month, xarinvh.xinvdate.day, xarinvh.xinvdate.year2 );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "<td>terms</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='InvoiceTerms'" );
	if ( nsStrlen ( xarinvh.xterms ) > 0 )
	{
		printf ( " value='%s'", xarinvh.xterms );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row - invoice status and invoice ponum
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>status</td>\n" );
	printf ( "<td>\n" );
	switch ( xarinvh.xstatus[0] )
	{
		case 'O': 
			printf ( "OPEN" ); 
			break;
		case 'P': 
			printf ( "PAID %02d/%02d/%02d", xarinvh.xpaydate.month, xarinvh.xpaydate.day, xarinvh.xpaydate.year2 ); 
			break;
		default : 
			printf ( "%s", xarinvh.xstatus ); 
			break;
	}
	printf ( "</td>\n" );
	printf ( "<td>po num</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='InvoicePoNum'" );
	if ( nsStrlen ( xarinvh.xponum ) > 0 )
	{
		printf ( " value='%s'", xarinvh.xponum );
	}
	printf ( ">\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		multiple rows - invoice lines
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='center'>line</td>\n" );
	printf ( "<td colspan='2'>description</td>\n" );
	printf ( "<td align='right'>amount</td>\n" );
	printf ( "</tr>\n" );

	snprintf ( WhereClause, sizeof(WhereClause), "arinvl.invoice = %ld", xarinvh.xid );
	LoadArinvlCB ( WhereClause, "arinvl.id", &xarinvl, (int(*)()) EachLine, 0 );

	/*----------------------------------------------------------
		invoice total
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='right' colspan='3'>invoice total</td>\n" );
	printf ( "<td align='right'>%.2f</td>\n", (double) InvoiceTotal / 100.0 );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		if payment, print payment and amount due
	----------------------------------------------------------*/
	if ( xarinvh.xpayment > 0 || xarinvh.xdiscount > 0 )
	{
		long		Balance;

		printf ( "<tr>\n" );
		printf ( "<td align='right' colspan='3'>discount</td>\n" );
		printf ( "<td align='right'>%.2f</td>\n", (double) xarinvh.xdiscount / 100.0 );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td align='right' colspan='3'>paid %s</td>\n", xarinvh.xrefnum );
		printf ( "<td align='right'>%.2f</td>\n", (double) xarinvh.xpayment / 100.0 );
		printf ( "</tr>\n" );

		Balance = InvoiceTotal - xarinvh.xdiscount - xarinvh.xpayment;

		printf ( "<tr>\n" );
		printf ( "<td align='right' colspan='3'>balance</td>\n" );
		printf ( "<td align='right'>%.2f</td>\n", (double) Balance / 100.0 );
		printf ( "</tr>\n" );
	}


	/*----------------------------------------------------------
		bottom row: save button
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='4'>\n" );

	printf ( "<input type='submit' name='submitClear' value='clear'>\n" );
	printf ( "&emsp;<input type='submit' name='submitFind' value='find'>\n" );
	printf ( "&emsp;<input type='submit' name='submitAddHead' value='add invoice'>\n" );
	printf ( "&emsp;<input type='submit' name='submitAddLine' value='add line'%s>\n", xarinvh.xid ? "" : "disabled" );
	printf ( "&emsp;<input type='submit' name='submitSave' value='save'%s>\n", xarinvh.xid ? "" : "disabled" );
	
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
