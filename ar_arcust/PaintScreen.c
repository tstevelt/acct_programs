/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

void PaintScreen ()
{
	printf ( "<table class='AppMedium'>\n" );

	printf ( "<tr>\n" );
	printf ( "<th class='maint' colspan='2' align='center'>\n" );
	printf ( "Customer Setup</th>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td width='25%%'>\n" );
	printf ( "ID Number</td>\n" );
	printf ( "<td width='74%%'>\n" );
	if ( RunMode == MODE_START )
	{
		printf ( "<input type='search' id='field_id' name='field_id' size='14' maxlength='11'>" );
		printf ( "&emsp;Leave blank to insert auto-generated ID.\n" );
	}
	else
	{
		printf ( "%ld", xarcust.xid );
		printf ( "<input type='hidden' name='field_id' value='%ld'>\n", xarcust.xid );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Name / Company</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_name' size='40' maxlength='60' value='%s'>\n", xarcust.xname );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Address 1</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_addr1' size='40' maxlength='60' value='%s'>\n", xarcust.xaddr1 );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Address 2</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_addr2' size='40' maxlength='60' value='%s'>\n", xarcust.xaddr2 );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "City</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_city' size='40' maxlength='40' value='%s'>\n", xarcust.xcity );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "State</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_state' size='7' maxlength='4' value='%s'>\n", xarcust.xstate );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Zipcode</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_zipcode' size='13' maxlength='10' value='%s'>\n", xarcust.xzipcode );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Phone</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_phone' size='20' maxlength='20' value='%s'>\n", xarcust.xphone );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>\n" );
	printf ( "Contact</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='field_contact' size='20' maxlength='20' value='%s'>\n", xarcust.xcontact );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td class='buttons' colspan='2'>\n" );
	switch ( RunMode )
	{
		case MODE_START:
			printf ( "<input type='reset'  value='reset'>\n" );
			printf ( "&emsp;<input type='submit' name='submitLookup' value='find'>\n" );
			printf ( "&emsp;<input type='submit' name='submitInsert' value='insert'>\n" );
			break;
		case MODE_FOUND:
			printf ( "<input type='submit' name='submitQuit' value='quit'>\n" );
			printf ( "&emsp;<input type='submit' name='submitUpdate' value='update'>\n" );
#ifdef SAFETY_ON_DELETE
			seed_random_with_usec ();
			HiddenSafety = random_range ( 123, 987 );

			printf ( "<input type='hidden' name='HiddenSafety' value='%d'>\n", HiddenSafety );
			printf ( "&emsp;%d", HiddenSafety );
			printf ( "&nbsp;<input type='search' name='AnswerSafety' size='4'>\n" );
			printf ( "&nbsp;" );
#else
			printf ( "&emsp;" );
#endif
			printf ( "<input type='submit' name='submitDelete' value='delete'>\n" );
			break;
		default:
			printf ( "Unknown run mode %d", RunMode );
			break;
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );
}
