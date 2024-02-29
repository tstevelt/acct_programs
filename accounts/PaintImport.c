/*----------------------------------------------------------------------------
	Program : PaintImport.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint import section of screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

int PaintImport ()
{
	/*--------------------------------------------------------------
		load import record.  if not found, return without
		painting anything
	--------------------------------------------------------------*/

	if ( LoadOneImport ( xaccount.xacctnum ) == 0 )
	{
		return ( 0 );
	}

	printf ( "<script language='JavaScript1.2'>\n" );
	printf ( "<!--\n" );
	printf ( "function OpenWebsite ( obj )\n" );
	printf ( "{\n" );
	printf ( "var command = obj.value;\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=600,height=500,resizable=1,menubar=1,scrollbars=1';\n" );
	printf ( "window.open ( command, 'impwin', options );\n" );
	printf ( "}\n" );
	printf ( "//-->\n" );
	printf ( "</script>\n" );

	printf ( "<input type='hidden' name='import_acctnum' value='%ld'>\n", ximport.xacctnum );

	printf ( "</table>\n" );
	printf ( "<table class='AppWide'>\n" );
	printf ( "<tr>\n" );
	printf ( "<th colspan='4'>web import setup </th>\n" );
	printf ( "</tr>\n" );

	/*--------------------------------------------------------------
		row one
	--------------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>web site</td>\n" );
	printf ( "<td colspan='3'>" );

	printf ( "<input type='search' name='website' size='60' maxlength='256' value='%s'>",
					ximport.xwebsite );

	printf ( "&nbsp;<input type='button' id='open_website' value='open'" );
	printf ( " onClick='javascript:OpenWebsite(website);'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*--------------------------------------------------------------
		row two
	--------------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>login</td>\n" );
	printf ( "<td>" );
	printf ( "<input type='search' name='login' size='20' maxlength='20' value='%s'> </td>\n",
					ximport.xlogin );

	printf ( "<td>last import date</td>\n" );

	printf ( "<td>" );
	printf ( "<input type='search' name='previous' size='10'" );
	if ( ximport.xprevious.month > 0 )
	{
		printf ( " value='%02d/%02d/%02d'",
				ximport.xprevious.month, ximport.xprevious.day, ximport.xprevious.year2 );
	}
	printf ( "></td>\n" );

	printf ( "</tr>\n" );

	/*--------------------------------------------------------------
		row three
	--------------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>import %s</td>\n", xaccount.xaccttype[0] == 'L' ? "payments" : "deposits" );
	printf ( "<td> <input type='checkbox' name='payments' %s> </td>\n", ximport.xpayments[0] == 'Y' ? "checked" : "" );

#ifdef VERSION_ONE
	printf ( "<td>auto parse</td>\n" );
	printf ( "<td> <input type='checkbox' name='autoparse' %s> </td>\n", 
			ximport.xautoparse[0] == 'Y' ? "checked" : "" );
#else
	printf ( "<td>reverse sign</td>\n" );
	printf ( "<td> <input type='checkbox' name='reverse' %s> </td>\n", ximport.xreverse[0] == 'Y' ? "checked" : "" );
#endif
	printf ( "</tr>\n" );


#ifdef VERSION_ONE
	if ( ximport.xautoparse[0] == 'N' )
	{
		/*--------------------------------------------------------------
			row four - custom parse
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );

		printf ( "<td>parse function</td>\n" );

		printf ( "<td>" );
		printf ( "<input type='search' name='parsefunc' size='20' value='%s'> </td>\n",
						ximport.xparsefunc );

		printf ( "<td colspan='2'>&nbsp;</td>" );

		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			hidden inputs for autoparse true
		--------------------------------------------------------------*/
		printf ( "<input type='hidden' name='headrows' value='%d'>\n", ximport.xheadrows );
		printf ( "<input type='hidden' name='colcount' value='%d'>\n", ximport.xcolcount );
		printf ( "<input type='hidden' name='refnum' value='%d'>\n", ximport.xrefnum );
		printf ( "<input type='hidden' name='date' value='%d'>\n", ximport.xdate );
		printf ( "<input type='hidden' name='payee' value='%d'>\n", ximport.xpayee );
		printf ( "<input type='hidden' name='increase' value='%d'>\n", ximport.xincrease );
		if ( ximport.xincsign[0] == 'Y' )
		{
			printf ( "<input type='hidden' name='incsign' value='1'> \n" );
		}
		printf ( "<input type='hidden' name='decrease' value='%d'>\n", ximport.xdecrease );
		if ( ximport.xdecsign[0] == 'Y' )
		{
			printf ( "<input type='hidden' name='decsign' value='1'> \n" );
		}
	}
	else
	{
		/*--------------------------------------------------------------
			row four - auto parse
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>header rows</td>\n" );
		printf ( "<td> <input type='search' name='headrows' size='6' value='%d'> </td>\n",
						ximport.xheadrows );
		printf ( "<td>column count</td>\n" );
		printf ( "<td> <input type='search' name='colcount' size='6' value='%d'> </td>\n",
						ximport.xcolcount );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			row five
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>reference number</td>\n" );
		printf ( "<td colspan='3'> <input type='search' name='refnum' size='6' value='%d'> </td>\n",
						ximport.xrefnum );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			row six
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>transaction date</td>\n" );
		printf ( "<td colspan='3'> <input type='search' name='date' size='6' value='%d'> </td>\n",
						ximport.xdate );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			row seven
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>payee</td>\n" );
		printf ( "<td colspan='3'> <input type='search' name='payee' size='6' value='%d'> </td>\n",
						ximport.xpayee );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			row eight
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>account increase</td>\n" );
		printf ( "<td> <input type='search' name='increase' size='6' value='%d'> </td>\n",
						ximport.xincrease );
		printf ( "<td>reverse sign</td>\n" );
		printf ( "<td> <input type='checkbox' name='incsign' %s> </td>\n", 
				ximport.xincsign[0] == 'Y' ? "checked" : "" );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			row nine
		--------------------------------------------------------------*/
		printf ( "<tr>\n" );
		printf ( "<td>account decrease</td>\n" );
		printf ( "<td> <input type='search' name='decrease' size='6' value='%d'> </td>\n",
						ximport.xdecrease );
		printf ( "<td>reverse sign</td>\n" );
		printf ( "<td> <input type='checkbox' name='decsign' %s> </td>\n", 
				ximport.xdecsign[0] == 'Y' ? "checked" : "" );
		printf ( "</tr>\n" );

		/*--------------------------------------------------------------
			hidden inputs for autoparse false
		--------------------------------------------------------------*/
		printf ( "<input type='hidden' name='parsefunc' value='%s'> </td>\n", ximport.xparsefunc );
	}
#endif

	return ( 1 );
}
