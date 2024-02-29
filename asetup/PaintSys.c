/*----------------------------------------------------------------------------
	Program : PaintSys.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint system record fields for editing.
	Return  : 
----------------------------------------------------------------------------*/

#include	"asetup.h"

// internal trxnext     | int(11)     |      |     | 1       |       |

void PaintSys ()
{
//	int		xt;

printf ( "\n<script language='JavaScript1.2'>\n" );
printf ( "<!-- \n" );

printf ( "function Clicker ( Tab )\n" );
printf ( "{\n" );
printf ( "var d = window.document;\n" );
printf ( "d.setup.what.value = 'save';\n" );
printf ( "d.setup.page.value = Tab;\n" );
printf ( "d.setup.submit();\n" );
printf ( "}\n" );

printf ( "// -->\n" );
printf ( "</script>\n" );

	printf ( "<table class='AppWide'0%%'>\n" );

	printf ( "<tr>\n" );

	printf ( "<td class='%s' align='center' width='33%%' ", PageNumber == 1 ? "TabSelected" : "TabOther" );
	printf ( " onClick='javascript:Clicker(1);'> name and address </td>\n" );
	printf ( "<td class='%s' align='center' width='33%%' ", PageNumber == 2 ? "TabSelected" : "TabOther" );
	printf ( " onClick='javascript:Clicker(2);'> options </td>\n" );
	printf ( "<td class='%s' align='center' width='33%%' ", PageNumber == 3 ? "TabSelected" : "TabOther" );
	printf ( " onClick='javascript:Clicker(3);'> users </td>\n" );

	printf ( "</tr>\n" );
	printf ( "</table>\n" );

	printf ( "<table class='AppWide'0%%'>\n" );

	switch ( PageNumber )
	{
		case 1:
// name     | char(30)    |      |     |         |       |
			printf ( "<tr>\n" );

			printf ( "<td> name </td> <td> <input type='search' name='name' size='30' maxlength='30' value='%s'> </td>\n",
							xsystem.xname );

			printf ( "</tr>\n" );
// address     | char(30)    |      |     |         |       |
			printf ( "<tr>\n" );

			printf ( "<td> address </td> <td> <input type='search' name='address' size='30' maxlength='30' value='%s'> </td>\n",
							xsystem.xaddress );
			printf ( "</tr>\n" );

// city     | char(30)    |      |     |         |       |
			printf ( "<tr>\n" );

			printf ( "<td> city </td> <td> <input type='search' name='city' size='30' maxlength='30' value='%s'> </td>\n",
							xsystem.xcity );
			printf ( "</tr>\n" );

// state    | char(20)    |      |     |         |       |
			printf ( "<tr>\n" );

			printf ( "<td> state </td> <td> <input type='search' name='state' size='20' maxlength='20' value='%s'> </td>\n",
							xsystem.xstate );
			printf ( "</tr>\n" );

// zipcode      | char(20)    |      |     |         |       |
			printf ( "<tr>\n" );

			printf ( "<td> zip </td> <td> <input type='search' name='zipcode' size='20' maxlength='20' value='%s'> </td>\n",
							xsystem.xzipcode );
			printf ( "</tr>\n" );

// phone    | char(20)    |      |     |         |       |
			printf ( "<tr>\n" );
			printf ( "<td> phone number </td>" );
			printf ( "<td> <input type='search' name='phone' size='20' maxlength='20' value='%s'>", xsystem.xphone );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td> two factor login </td>" );
			printf ( "<td>" );
			printf ( "<select name='twofactor'>" );
			printf ( "<option value='Y'%s>Yes</option>\n", xsystem.xtwofactor[0] == 'Y' ? " selected" : "" );
			printf ( "<option value='N'%s>No</option>\n",  xsystem.xtwofactor[0] != 'Y' ? " selected" : "" );
			printf ( "</select>" );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );


					
			break;

		case 2:
// assetnext   | smallint(6) |      |     | 1001    |       |
// showamts    | char(1)     |      |     | Y          |       |
			printf ( "<tr>\n" );
			printf ( "<td> asset </td> <td> <input type='search' name='assetnext' size='6' value='%d'> </td>\n",
							xsystem.xassetnext );

			printf ( "<td> show </td>\n" );
			printf ( "<td>\n" );
			printf ( "<select name='showamts'>\n" );
			printf ( "<option value='A'%s>show amts</option>\n", xsystem.xshowamts[0] == 'A' ? " selected" : "" );
			printf ( "<option value='C'%s>trx count</option>\n", xsystem.xshowamts[0] == 'C' ? " selected" : "" );
			printf ( "<option value='B'%s>both</option>\n",      xsystem.xshowamts[0] == 'B' ? " selected" : "" );
			printf ( "<option value='N'%s>nothing</option>\n",   xsystem.xshowamts[0] == 'N' ? " selected" : "" );
			printf ( "</select>\n" );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );

// liabnext    | smallint(6) |      |     | 2001    |       |
			printf ( "<tr>\n" );
			printf ( "<td> liability </td> <td> <input type='search' name='liabnext' size='6' value='%d'> </td>\n",
							xsystem.xliabnext );

			printf ( "<td> print dest </td>\n" );
			printf ( "<td>\n" );
			printf ( "<select name='printdest'>\n" );
			printf ( "<option value='S'%s>server printer</option>\n", 
						xsystem.xprintdest[0] == 'S' ? " selected" : "" );
			printf ( "<option value='L'%s>local printer</option>\n",
						xsystem.xprintdest[0] == 'S' ? "" : " selected" );
			printf ( "</select>\n" );
			printf ( "</td>\n" );

			printf ( "</tr>\n" );

// equitynext  | smallint(6) |      |     | 3001    |       |
// printdest   | char(1)     |      |     | S          |       |
			printf ( "<tr>\n" );
			printf ( "<td> equity </td> <td> <input type='search' name='equitynext' size='6' value='%d'> </td>\n",
							xsystem.xequitynext );

// next trx number
			printf ( "<td> next trx </td>" );
			printf ( "<td> %ld", xsystem.xtrxnext );
			printf ( "</td>\n" );

			printf ( "</tr>\n" );

// incomenext  | smallint(6) |      |     | 4001    |       |
			printf ( "<tr>\n" );
			printf ( "<td> income </td> <td> <input type='search' name='incomenext' size='6' value='%d'> </td>\n",
							xsystem.xincomenext );

// number of trx on entry
			printf ( "<td> number trx on new entry </td>" );
			printf ( "<td> <input type='search' name='numtrxnew' size='5' value='%d'>", xsystem.xnumtrxnew );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );

			printf ( "</tr>\n" );

// expensenext | smallint(6) |      |     | 6001    |       |
			printf ( "<tr>\n" );
			printf ( "<td> expense </td> <td> <input type='search' name='expensenext' size='6' value='%d'> </td>\n",
							xsystem.xexpensenext );

// number of trx on entry
			printf ( "<td> number trx on edit entry </td>" );
			printf ( "<td> <input type='search' name='numtrxedit' size='5' value='%d'>", xsystem.xnumtrxedit );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );




/*---------------------------------------------------------------------------
	Last Close Date
	Reconcile before close
	Close method
	Manual - Balance Sheet must net to Zero a closing date
	Automatic
		Retained Earning Equity Account
		Retained Earnings Income Contra Account
---------------------------------------------------------------------------*/
			printf ( "<tr>\n" );
			printf ( "<td class='TabSelected' colspan='4' align='center'>\n" ); 
			printf ( "the options below all apply to closing an accounting period." );
			printf ( "</td>\n" );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );

// closedate   | date        |      |     | 0000-00-00 |       |
			printf ( "<td> last close date </td>" );
			if ( xsystem.xclosedate.month > 0 )
			{
				printf ( "<td> %02d/%02d/%04d </td>\n" ,
							xsystem.xclosedate.month, xsystem.xclosedate.day, xsystem.xclosedate.year4 );
			}
			else
			{
				printf ( "<td>&nbsp;</td>\n" );
			}

// reconcile   | char(1)     |      |     | N          |       |
			printf ( "<td> reconcile all transactions before close </td>\n" );
			printf ( "<td>\n" );
			printf ( "<select name='reconcile'>\n" );
			printf ( "<option value='Y'%s>yes</option>\n", xsystem.xreconcile[0] == 'Y' ? " selected" : "" );
			printf ( "<option value='N'%s>no</option>\n", xsystem.xreconcile[0] == 'Y' ? "" : " selected" );
			printf ( "</select>\n" );
			printf ( "</td>\n" );

			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td>close method</td>\n" );
			printf ( "<td>\n" );
			printf ( "<select name='closemethod'>\n" );
			printf ( "<option value='M'%s>manual</option>\n",    xsystem.xclosemethod[0] == 'M' ? " selected" : "" );
			printf ( "<option value='A'%s>automatic</option>\n", xsystem.xclosemethod[0] == 'M' ? "" : " selected" );
			printf ( "</select>\n" );
			printf ( "</td>\n" );

			printf ( "<td colspan='2'>&nbsp;</td>\n" );

			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td>&nbsp;</td>\n" );
			printf ( "<td>manual</td>\n" );
			printf ( "<td colspan='2'>balance sheet must net to zero for new closing date </td>\n" );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td>&nbsp;</td>\n" );
			printf ( "<td>automatic</td>\n" );
			printf ( "<td colspan='2'>net income for the period is posted to these two accounts.</td>\n" );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td colspan='2'>&nbsp;</td>\n" );
			printf ( "<td>retained earnings equity account</td>\n" );
			printf ( "<td> <input type='search' name='retainequity' size='6' value='%d'> </td>\n", xsystem.xretainequity );
			printf ( "</tr>\n" );

			printf ( "<tr>\n" );
			printf ( "<td colspan='2'>&nbsp;</td>\n" );
			printf ( "<td>retained earnings income contra account</td>\n" );
			printf ( "<td> <input type='search' name='retainincome' size='6' value='%d'> </td>\n", xsystem.xretainincome );
			printf ( "</tr>\n" );

			break;

		case 3:
			PaintUsers ();
			break;

	}

// bottom row: save button
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='6'>\n" );
	printf ( "<input type='button' value='save' " );
	if ( xmember.xmrole[0] == ROLE_DEMO )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:what.value=\"save\";submit();'> &nbsp;\n" );
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
