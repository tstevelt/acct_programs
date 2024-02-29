/*----------------------------------------------------------------------------
	Program : PaintTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint transactions
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

#include	"entry.h"

extern	int	FirstTrx;
extern	int	LastTrx;

void PaintTrx ()
{
	long	TrxBalance = 0;
	int		xa, xt;
	char	FieldNameOne[20];
	char	FieldNameTwo[20];
	int		NewSeq, LastSeq;
	int		ThisIsANewTrx;
	int		PaintAccessKey;

	if ( xtrxh.xtrxnum == 0 /* || TrxArray[1].d.xacctnum == 0 */ )
	{
		ThisIsANewTrx = 1;
	}
	else
	{
		ThisIsANewTrx = 0;
	}

	/*--------------------------------------------------------------
		category lookup function
	--------------------------------------------------------------*/
	webStartJava ();

	printf ( "function LookupCategory ( Pattern, TargetNumber, TargetName )\n" );
	printf ( "{\n" );
	printf ( "if ( Pattern.value == '' )" );
	printf ( "{\n" );
	printf ( "alert ( 'Enter at least first letter!' );\n" );
	printf ( "return;\n" );
	printf ( "}\n" );
	printf ( "window.open('entry.cgi?lookup=' + Pattern.value + '&targetNumber=' + TargetNumber +  '&targetName=' + TargetName, 'lookup', " );
	printf ( "'screenX=30,screenY=30,width=640,height=500,resizable=1,menubar=0,scrollbars=1' );\n" );
	printf ( "}\n" );
	
	webEndJava ();

	if ( ThisIsANewTrx == 0 )
	{
		LoadTrx ( xtrxh.xtrxnum );
	}

	/*--------------------------------------------------------------
		primary account
	--------------------------------------------------------------*/
	printf ( "<table class='AppWide'>\n" );

	/*--------------------------------
		row one
	--------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>account</td>\n" );
	printf ( "<td colspan='3'>\n" );
	if ( ThisIsANewTrx == 1 )
	{
		LoadAccounts ( "(accttype = 'A' or accttype = 'L' or accttype = 'E')" );

		printf ( "<select name='acctnum_insert_1'>\n" );

		printf ( "<option value='0'>--- choose account ---</option>\n" );

		for ( xa = 0; xa < AcctsCount; xa++ )
		{
			printf ( "<option value='%ld'>%s ",
							AcctsArray[xa].xacctnum,
							AcctsArray[xa].xacctname );
			
			switch ( AcctsArray[xa].xaccttype[0] )
			{
				case 'A':
					printf ( "(asset)" );
					break;
				case 'L':
					printf ( "(liability)" );
					break;
				case 'E':
					printf ( "(equity)" );
					break;
			}
			
			printf ( "</option>\n" );
		}
		printf ( "</select>\n" );
	}
	else
	{
		LoadOneAccount ( TrxArray[1].d.xacctnum );
		printf ( "%s", xaccount.xacctname );
		printf ( "<input type='hidden' name='acctnum_update_1' value='%ld'>", xaccount.xacctnum );
	}
	printf ( "</td>\n" );

	printf ( "<td>\n" );
	printf ( "trx\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	if ( ThisIsANewTrx == 1 )
	{
		/*--------------------------------------------------------------
		xxx	printf ( "new\n" );
		xxx	printf ( "<input type='hidden' name='trxnum' value='0'>" );
		--------------------------------------------------------------*/
		printf ( "<input type='search' id='trxnum' name='trxnum' size='8'>" );
	}
	else
	{
		printf ( "%ld\n", xtrxh.xtrxnum );
		printf ( "<input type='hidden' id='trxnum' name='trxnum' value='%ld'>", xtrxh.xtrxnum );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*--------------------------------
		row two
	--------------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>reference</td>\n" );
	printf ( "<td><input type='search' name='refnum_1' size='8' value='%s'> </td>\n", 
					xtrxh.xrefnum );

	printf ( "<td>date</td>\n" );
	printf ( "<td><input type='search' name='trxdate_1' size='10' " );
	if ( xtrxh.xtrxdate.month > 0 )
	{
		printf ( "value='%d/%d/%d'",
					xtrxh.xtrxdate.month,
					xtrxh.xtrxdate.day,
					xtrxh.xtrxdate.year2  );
	}
	printf ( "></td>\n" );

	printf ( "<td>status</td>\n" );

	switch ( xtrxh.xstatus )
	{
		case STATUS_PREENT : printf ( "<td bgcolor='%s'>", COLOR_PREENT ); break;
		case STATUS_ACCEPT : printf ( "<td bgcolor='%s'>", COLOR_ACCEPT ); break;
		case STATUS_BALANCE: printf ( "<td bgcolor='%s'>", COLOR_BALANCE ); break;
		default            : printf ( "<td>" ); break;
	}
	printf ( "%s</td>\n", StatusDescr ( xtrxh.xstatus, 2 )  );

	printf ( "</tr>\n" );

	/*--------------------------------
		row three
	--------------------------------*/
	printf ( "<tr>\n" );

	printf ( "<td>payee</td>\n" );
	printf ( "<td colspan='3'><input type='search' name='payee_1' size='40' value='%s'> </td>\n",							TrxArray[1].d.xpayee );

	printf ( "<td>amount</td>\n" );
	printf ( "<td><input type='search' name='amount_1' size='10' value='%.2f'> </td>\n",
					TrxArray[1].d.xamount / 100.0 );

	printf ( "</tr>\n" );

	printf ( "</table>\n" );

	/*--------------------------------------------------------------
		offsetting category(s) 
	--------------------------------------------------------------*/
	printf ( "<table class='AppWide'>\n" );
	/*--------------------------------
		row four
	--------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<th>categ_ory</th>\n" );
	printf ( "<th>memo</th>\n" );
	printf ( "<th>_amount</th>\n" );
	printf ( "<th>del</th>\n" );
	printf ( "</tr>\n" );

	PaintAccessKey = 1;

	for ( xt = 2; xt <= TrxCount; xt++ )
	{
		/*--------------------------------
			row five ... n
		--------------------------------*/
		printf ( "<tr>\n" );

		/*--------------------------------------------------------------
			category account number and lookup button
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );

		sprintf ( FieldNameOne, "acctnum_update_%d", TrxArray[xt].d.xseqnum );
		printf ( "<input type='hidden' name='%s' value='%ld'>\n", 
				FieldNameOne, TrxArray[xt].d.xacctnum );

		sprintf ( FieldNameTwo, "acctname_update_%d", TrxArray[xt].d.xseqnum );
		printf ( "<input type='search' name='%s' size='30' value='%s'", 
				FieldNameTwo, TrxArray[xt].c.xacctname );
		printf ( " onChange='javascript:%s.value=%s.value;'%s", 
					FieldNameOne, FieldNameTwo,
					PaintAccessKey ? " accesskey='o'" : "" );
		printf ( ">" );

		printf ( "&nbsp;<input type='button' value='?' accesskey='%d' ", xt - 1 );
		printf ( " onClick='javascript:LookupCategory(%s,\"%s\",\"%s\");'",
					FieldNameTwo, FieldNameOne, FieldNameTwo );

		printf ( "</td>\n" );

		/*--------------------------------------------------------------
			category payee/memo
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );
		printf ( "<input type='search' name='payee_%d' size='30' value='%s'>", 
				TrxArray[xt].d.xseqnum, TrxArray[xt].d.xpayee );
		printf ( "</td>\n" );

		/*--------------------------------------------------------------
			category amount
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );
		printf ( "<input type='search' name='amount_%d' size='10' value='%.2f'%s>", 
				TrxArray[xt].d.xseqnum, (double) TrxArray[xt].d.xamount / 100.0,
				PaintAccessKey ? " accesskey='a'" : "" );
		printf ( "</td>\n" );

		/*--------------------------------------------------------------
			delete checkbox
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );
		if ( xtrxh.xstatus < STATUS_RECONCILED )
		{
			printf ( "<input type='checkbox' name='delete_%d'>", TrxArray[xt].d.xseqnum );
		}
		else
		{
			printf ( "&nbsp;" );
		}
		printf ( "</td>\n" );

		printf ( "</tr>\n" );

		if ( PaintAccessKey == 1 )
		{
			PaintAccessKey = 0;
		}
	}

	/*--------------------------------------------------------------
		add one (or more, see system.numtrxnew) blank transaction
		if new transaction  paint six blank rows.
		if existing transaction then paint just one blank row.
		tms 11/24/2020 edit trans now used new field numtrxedit.
	--------------------------------------------------------------*/
	if ( ThisIsANewTrx == 1 )
	{
		NewSeq = 2;
		LastSeq = NewSeq + xsystem.xnumtrxnew - 1;
	}
	else
	{
		NewSeq = TrxCount + 1;
		// LastSeq = NewSeq;
		LastSeq = NewSeq + xsystem.xnumtrxedit - 1;
	}

	for ( ; NewSeq <= LastSeq; NewSeq++ )
	{
		printf ( "<tr>\n" );

		/*--------------------------------------------------------------
			category account number and lookup button
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );

		sprintf ( FieldNameOne, "acctnum_insert_%d", NewSeq );
		printf ( "<input type='hidden' name='%s'>\n", FieldNameOne );

		sprintf ( FieldNameTwo, "acctname_insert_%d", NewSeq );
		printf ( "<input type='search' name='%s' size='30' ", FieldNameTwo );
		printf ( " onChange='javascript:acctnum_insert_%d.value=acctname_insert_%d.value;'%s", 
						NewSeq, NewSeq,
						PaintAccessKey ? " accesskey='o'" : "" );
		printf ( ">\n" );

		printf ( "&nbsp;<input type='button' value='?' " );
		printf ( " onClick='javascript:LookupCategory(%s,\"%s\",\"%s\");'>", 
							FieldNameTwo, FieldNameOne, FieldNameTwo );
		printf ( "</td>\n" );

		/*--------------------------------------------------------------
			category payee/memo
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );
		printf ( "<input type='search' name='payee_%d' size='30'>", NewSeq );
		printf ( "</td>\n" );

		/*--------------------------------------------------------------
			category amount
		--------------------------------------------------------------*/
		printf ( "<td align='center'>" );
		printf ( "<input type='search' name='amount_%d' size='10'%s>", 
				NewSeq,
				PaintAccessKey ? " accesskey='a'" : "" );
		printf ( "</td>\n" );

		printf ( "<td>&nbsp;</td>\n" );

		printf ( "</tr>\n" );

		if ( PaintAccessKey == 1 )
		{
			PaintAccessKey = 0;
		}
	}


	/*--------------------------------------------------------------
		bottom row
	--------------------------------------------------------------*/
	printf ( "<tr>\n" );

	/*--------------------------------------------------------------
		navigation buttons
	--------------------------------------------------------------*/
	printf ( "<td align='center'>\n" );

	printf ( "<input type='button' value='_clear' accesskey='c' " );
	printf ( "onClick='javascript:what.value=\"clear\";submit();'>&nbsp;\n" );

	printf ( "<input type='button' value='_find' accesskey='f' " );
	printf ( "onClick='javascript:what.value=\"find\";submit();'>&nbsp;\n" );

	printf ( "<input type='button' value='_prev' accesskey='p' " );
	if ( FirstTrx == 0 && ThisIsANewTrx == 0 )
	{
		printf ( "onClick='javascript:what.value=\"prev\";submit();'>&nbsp;\n" );
	}
	else
	{
		printf ( " disabled>\n" );
	}

#ifdef FIREFOX_GOOD_NOW
	printf ( "<input type='button' value='_next' accesskey='n' " );
#else
	printf ( "<input type='button' value='ne_xt' accesskey='x' " );
#endif
	if ( LastTrx == 0 && ThisIsANewTrx == 0 )
	{
		printf ( "onClick='javascript:what.value=\"next\";submit();'>&nbsp;\n" );
	}
	else
	{
		printf ( " disabled>\n" );
	}

	printf ( "<input type='button' value='_save' accesskey='s' " );
	if ( xmember.xmrole[0] == ROLE_DEMO || xtrxh.xstatus == STATUS_CLOSED  )
	{
		printf ( "disabled>&nbsp;\n" );
	}
	else
	{
		printf ( "onClick='javascript:what.value=\"save\";submit();'>&nbsp;\n" );
	}

	printf ( "</td>\n" );

	/*--------------------------------------------------------------
		delete and save buttons
	--------------------------------------------------------------*/
	printf ( "<td align='center'>\n" );

	if ( xmember.xmrole[0] != ROLE_DEMO && xtrxh.xtrxnum > 0 && xtrxh.xstatus < STATUS_RECONCILED )
	{
		HiddenCaptcha = (int) random_range ( 112, 987 );
		printf ( "<input type='hidden' name='HiddenCaptcha' value='%d'>\n", HiddenCaptcha );
		printf ( "%d&nbsp;", HiddenCaptcha );
		printf ( "<input type='search' name='DeleteCaptcha' size='4'>&emsp;\n" );
	}

	printf ( "<input type='button' value='delete' " );
	if ( xmember.xmrole[0] != ROLE_DEMO && xtrxh.xtrxnum > 0 && xtrxh.xstatus < STATUS_RECONCILED )
	{
		printf ( " onClick='javascript:what.value=\"delete\";submit();'>&nbsp;\n" );
	}
	else
	{
		printf ( " disabled>\n" );
	}

	/*----------------------------------------------------------
		tms 12/03/2020 moved save button to left with nav buttons
	----------------------------------------------------------*/
	printf ( "</td>\n" );

	/*--------------------------------------------------------------
		transaction balance
	--------------------------------------------------------------*/

	if ( xtrxh.xtrxnum > 0 )
	{
		TrxBalance = CalcTrxBalance ( xtrxh.xtrxnum );
	}
	printf ( "<td align='center'" );
	if ( TrxBalance != 0L )
	{
		printf ( " bgcolor='%s'", COLOR_BALANCE );
	}
	printf ( ">%.2f</td>\n", (double) TrxBalance / 100.0 );

	printf ( "<td>&nbsp;</td>\n" );

	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
