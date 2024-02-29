/*----------------------------------------------------------------------------
	Program : PaintTop.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Start screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"reconcile.h"

void PaintTop ()
{
	int		xa, xp;
	DATEVAL	NewStartDate;

	LoadAccounts ( "(accttype = 'A' or accttype = 'L')" );

	printf ( "<table class='AppWide'>\n" );

	printf ( "<tr>\n" );

	if ( RunMode == MODE_START )
	{
		printf ( "<td>account</td>\n" );
		printf ( "<td colspan='4'>\n" );
		printf ( "<select name='acctnum' " );
		printf ( " onChange='javascript:SetStartDate(this,StartDate,StartBalance);'>\n" );

		printf ( "<option value='0'>--- choose account ---</option>\n" );

		for ( xa = 0; xa < AcctsCount; xa++ )
		{
			printf ( "<option value='%ld'>%s (%s)</option>\n",
							AcctsArray[xa].xacctnum,
							AcctsArray[xa].xacctname,
							AcctsArray[xa].xaccttype[0] == 'A' ? "asset" : "liabilty" );
		}
		printf ( "</select>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );
	}
	else
	{
		LoadOneAccount ( xaccount.xacctnum );
		printf ( "<th colspan='2'>\n" );
		printf ( "%s", xaccount.xacctname );
		printf ( "<input type='hidden' name='acctnum' value='%ld'>", xaccount.xacctnum );
		printf ( "</th>\n" );
	}

	if ( RunMode == MODE_START )
	{
		printf ( "<tr>\n" );
		printf ( "<td>starting date</td>\n" );
		printf ( "<td><input type='search' id='StartDate' name='StartDate' size='10'> </td>\n" );
		printf ( "<td>starting balance</td>\n" );
		printf ( "<td><input type='search' name='StartBalance' id='StartBalance' size='10'> </td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td>ending date</td>\n" );
		printf ( "<td><input type='search' name='EndDate' size='10'> </td>\n" );
		printf ( "<td>ending balance</td>\n" );
		printf ( "<td><input type='search' name='EndBalance' id='EndBalance' size='10'> </td>\n" );
		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td align='center' colspan='4'>" );

		printf ( "<input type='button' value='get' " );
		printf ( " onClick='javascript:what.value=\"get\";submit();'>\n" );

		printf ( "</td>\n" );
		printf ( "</tr>\n" );
	}
	else
	{
		printf ( "<td>starting date</td>\n" );
		printf ( "<td>%02d/%02d/%04d</td>\n",
							StartDate.month, StartDate.day, StartDate.year4 );
		printf ( "<input type='hidden' name='StartDate' value='%02d/%02d/%04d'>\n", 
							StartDate.month, StartDate.day, StartDate.year4 );

		printf ( "<td>starting balance</td>\n" );
		printf ( "<td>%.2f</td>\n", (double) StartBalance / 100.0 );
		printf ( "<input type='hidden' name='StartBalance' value='%.2f'>\n", 
									(double) StartBalance / 100.0 );

		printf ( "</tr>\n" );

		printf ( "<tr>\n" );
		printf ( "<td colspan='2'>&nbsp;</td>\n" );

		printf ( "<td>ending date</td>\n" );
		printf ( "<td>%02d/%02d/%04d</td>\n",
							EndDate.month, EndDate.day, EndDate.year4 );
		printf ( "<input type='hidden' name='EndDate' value='%02d/%02d/%04d'>\n", 
							EndDate.month, EndDate.day, EndDate.year4 );


		printf ( "<td>ending balance</td>\n" );
		printf ( "<td>%.2f</td>\n", (double) EndBalance / 100.0 );
		printf ( "<input type='hidden' name='EndBalance' value='%.2f'>\n", 
									(double) EndBalance / 100.0 );

		printf ( "</tr>\n" );
	}

	printf ( "</table>\n" );

	if ( RunMode == MODE_START )
	{
		printf ( "<script language='JavaScript1.1'>\n" );
		printf ( "<!--\n" );
		printf ( "var acctdetail = [ \n" );

		for ( xa = 0, xp = 0; xa < AcctsCount; xa++ )
		{
			if ( AcctsArray[xa].xrecondate.month > 0 )
			{
				xp++;

				/*--------------------------------------------------------------
					had to output account numbers as string (with quotes) 
					instead of numbers, when I moved the function from
					here into the .js file.  seems weird.
				--------------------------------------------------------------*/
//					AcctsArray[xa].xrecondate.month,
// AcctsArray[xa].xrecondate.day < 28 ? AcctsArray[xa].xrecondate.day + 1 : AcctsArray[xa].xrecondate.day,
//					AcctsArray[xa].xrecondate.year2,

//				int DateAdd ( DATEVAL *a, int NumberOfDays, DATEVAL *b )
				DateAdd ( &AcctsArray[xa].xrecondate, 1, &NewStartDate );

				printf ( "[ '%ld', '%02d/%02d/%02d', '%ld' ]%c\n",
					AcctsArray[xa].xacctnum,
					NewStartDate.month,
					NewStartDate.day,
					NewStartDate.year2,
					AcctsArray[xa].xreconbal,
					xa + 1 < AcctsCount ? ',' : ' ' );
			}
		}

		/*--------------------------------------------------------------
			print at least one dummy element in array to prevent
			javascript errors.
		--------------------------------------------------------------*/
		if ( xp == 0 )
		{
			xp++;
			printf ( "[ 0, '00/00/00' ]\n" );
		}

		printf ( "];\n" );

		printf ( "var acctcount = %d\n", xp );

		printf ( "//-->\n" );
		printf ( "</script>\n" );

	}
}
