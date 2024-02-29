/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"chpw.h"

void PaintScreen ()
{
	if ( RunMode == MODE_PAINT_CHANGE )
	{
		printf ( "<input type='hidden' name='Database' value='%s'>\n", SessionRecord.DbName );
		printf ( "<input type='hidden' name='MemberName' value='%s'>\n", SessionRecord.UserName );
	}

	printf ( "<table class='AppWide'0%%'>\n" );

	if ( RunMode == MODE_PAINT_CHANGE )
	{
		printf ( "<tr>\n" );
		printf ( "<td>Current Password</td>\n" );
		printf ( "<td>\n" );
		printf ( "<input type='password' name='CurrentPassword'>\n" );
		printf ( "</td>\n" );
		printf ( "</tr>\n" );

	}
	else
	{
		printf ( "<input type='hidden' name='Database' value='%s'>\n", Database );
		printf ( "<input type='hidden' name='MemberName' value='%s'>\n", MemberName );
	}

	printf ( "<tr>\n" );
	printf ( "<td>New Password</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='password' name='NewPassword'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>Confirm Password</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='password' name='ConfirmPassword'>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

// bottom row: save button
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='6'>\n" );
	if ( RunMode == MODE_PAINT_CHANGE )
	{
		printf ( "<input type='submit' name='submitSaveChange' value='save'>" );
	}
	else
	{
		printf ( "<input type='submit' name='submitSaveForgot' value='save'>" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );
}
