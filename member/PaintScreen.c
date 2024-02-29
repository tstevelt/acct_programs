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

#include	"member.h"

void PaintScreen ()
{
	int		ndx;

	printf ( "<table class='AppMedium'>\n" );

	printf ( "<tr>\n" );
	printf ( "<th class='maint' colspan='2' align='center'>\n" );
	printf ( "Member Setup</th>\n" );
	printf ( "</tr>\n" );

	/*------------------------------------------------------------
	generated paint
------------------------------------------------------------*/

/*--------------------
	Row 1
--------------------*/
printf ( "<tr>\n" );
printf ( "<td width='25%%'>\n" );
printf ( "ID</td>\n" );
printf ( "<td width='74%%'>\n" );
if ( RunMode == MODE_START )
{
	printf ( "<input type='search' id='field_id' name='field_id' size='14' maxlength='11'>" );
	printf ( "&emsp;Leave blank to insert auto-generated ID.\n" );
}
else
{
	printf ( "%ld", xmember.xid );
	printf ( "<input type='hidden' name='field_id' value='%ld'>\n", xmember.xid );
}
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 2
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Name</td>\n" );
printf ( "<td>\n" );
printf ( "<input type='search' name='field_mname' size='33' maxlength='30'" );
if ( nsStrlen ( xmember.xmname ) > 0 )
{
	printf ( " value='%s'", xmember.xmname );
}
printf ( ">\n" );
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 3
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Email</td>\n" );
printf ( "<td>\n" );
printf ( "<input type='search' name='field_memail' size='50' maxlength='80'" );
if ( nsStrlen ( xmember.xmemail ) > 0 )
{
	printf ( " value='%s'", xmember.xmemail );
}
printf ( ">\n" );
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 4
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Phone</td>\n" );
printf ( "<td>\n" );
printf ( "<input type='search' name='field_mphone' size='15' maxlength='12'" );
if ( nsStrlen ( xmember.xmphone ) > 0 )
{
	printf ( " value='%s'", xmember.xmphone );
}
printf ( ">\n" );
printf ( "<select name='field_mcarrier'>\n" );
printf ( "<option value='?'>-- select --</option>\n" );
for ( ndx = 0; ndx < CarrierCount; ndx++ )
{
	printf ( "<option value='%s'%s>%s</option>\n", 
		CarrierArray[ndx].Label, 
		nsStrcmp(CarrierArray[ndx].Label,xmember.xmcarrier) ? "" : " selected",
		CarrierArray[ndx].Label );
}
printf ( "</select>\n" );
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 5
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Two factor</td>\n" );
printf ( "<td>\n" );
printf ( "<select name='field_mtwopref'>\n" );
printf ( "<option value='?'>-- select --</option>\n" );
printf ( "<option value='N'%s>None</option>\n",  xmember.xmtwopref[0] == 'N' ? " selected" : "" );
printf ( "<option value='E'%s>Email</option>\n", xmember.xmtwopref[0] == 'E' ? " selected" : "" );
printf ( "<option value='P'%s>Phone</option>\n", xmember.xmtwopref[0] == 'P' ? " selected" : "" );
printf ( "</select>\n" );
printf ( "</td>\n" );
printf ( "</tr>\n" );



/*--------------------
	Row 6
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Role</td>\n" );
printf ( "<td>\n" );
printf ( "<select name='field_mrole'>\n" );
printf ( "<option value='?'>-- select --</option>\n" );
printf ( "<option value='D'%s>Demo</option>\n",          xmember.xmrole[0] == 'D' ? " selected" : "" );
printf ( "<option value='M'%s>Member</option>\n",        xmember.xmrole[0] == 'M' ? " selected" : "" );
printf ( "<option value='A'%s>Administrator</option>\n", xmember.xmrole[0] == 'A' ? " selected" : "" );
printf ( "</select>\n" );
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 7
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Added Date</td>\n" );
printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
printf ( "<input type='search' name='field_minsdt' size='13' maxlength='10'" );
if ( xmember.xminsdt.month >  0 )
{
	printf ( " value='%02d/%02d/%02d'", xmember.xminsdt.month, xmember.xminsdt.day, xmember.xminsdt.year2 );
}
printf ( ">\n" );
	}
	else
	{
		printf ( "%02d/%02d/%02d", xmember.xminsdt.month, xmember.xminsdt.day, xmember.xminsdt.year2 );
	}
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 8
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Password</td>\n" );
printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
		printf ( "<input type='search' name='field_mpassword' size='45' maxlength='42'" );
		if ( nsStrlen ( xmember.xmpassword ) >  0 )
		{
			printf ( " value='%s'", xmember.xmpassword );
		}
		printf ( ">\n" );
	}
	else
	{
		printf ( "%s", xmember.xmpassword );
	}
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 9
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Login IP</td>\n" );
printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
printf ( "<input type='search' name='field_mipaddr' size='19' maxlength='16'" );
if ( nsStrlen ( xmember.xmipaddr ) > 0 )
{
	printf ( " value='%s'", xmember.xmipaddr );
}
printf ( ">\n" );
	}
	else
	{
		printf ( "%s", xmember.xmipaddr );
	}
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 10
--------------------*/
printf ( "<tr>\n" );
printf ( "<td>\n" );
printf ( "Login Date</td>\n" );
printf ( "<td>\n" );
	if ( RunMode == MODE_START )
	{
printf ( "<input type='search' name='field_mlogdt' size='13' maxlength='10'" );
if ( xmember.xmlogdt.month >  0 )
{
	printf ( " value='%02d/%02d/%02d'", xmember.xmlogdt.month, xmember.xmlogdt.day, xmember.xmlogdt.year2 );
}
printf ( ">\n" );
	}
	else
	{
		printf ( "%02d/%02d/%02d", xmember.xmlogdt.month, xmember.xmlogdt.day, xmember.xmlogdt.year2 );
	}
printf ( "</td>\n" );
printf ( "</tr>\n" );

/*--------------------
	Row 11
--------------------*/
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
			printf ( "<input type='submit' name='submitQuit' value='clear'>\n" );
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
