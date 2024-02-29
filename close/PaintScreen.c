/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"close.h"

void PaintScreen ()
{
	printf ( "<table class='AppHalf'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>current close date</td>\n" );
	printf ( "<td>\n" );
	if ( xsystem.xclosedate.month > 0 )
	{
		printf ( "%02d/%02d/%02d\n",
				xsystem.xclosedate.month, xsystem.xclosedate.day, xsystem.xclosedate.year2 );
	}
	else
	{
		printf ( "&nbsp;" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>close date</td>\n" );
	printf ( "<td><input type='search' name='closedate' size='10'></td>\n" );
	printf ( "</tr>\n" );


	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='2'>\n" );

	if ( xmember.xmrole[0] == ROLE_ADMIN )
	{
		printf ( "<input type='button' value='close' " );
		printf ( "onClick='javascript:what.value=\"close\";submit();'>\n" );
	}
	else
	{
		printf ( "close can only be run by an administrator.\n" );
	}

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
