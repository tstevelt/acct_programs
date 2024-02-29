/*----------------------------------------------------------------------------
	Program : Lookup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Print list of members matching user criteria
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

static XMEMBER LookupMember;

static int MyEachMember ( XMEMBER *ptrMember )
{
	static int firstpass = 1;

	if ( firstpass )
	{
		firstpass = 0;

		printf ( "<table class='AppWide'>\n" );
		printf ( "<tr>\n" );

		printf ( "<td>ID</td>\n" );
printf ( "<td>Name</td>\n" );
printf ( "<td>Email</td>\n" );
printf ( "<td>Role</td>\n" );
printf ( "<td>Login Date</td>\n" );

		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>" );
printf ( "<a href='member.cgi?submitLoad=load&field_id=%ld'>%ld</a>", ptrMember->xid, ptrMember->xid );
printf ( "</td>\n" );
printf ( "<td>%s</td>\n", ptrMember->xmname );
printf ( "<td>%s</td>\n", ptrMember->xmemail );
printf ( "<td>" );
	switch ( ptrMember->xmrole[0] )
	{
		case 'D': printf ( "Demo" ); break;
		case 'M': printf ( "Member" ); break;
		case 'A': printf ( "Admin" ); break;
		default : printf ( "not set!" ); break;
	}
printf ( "</td>\n" );
if ( ptrMember->xmlogdt.month > 0 )
{
	printf ( "<td>%d/%d/%d</td>\n", ptrMember->xmlogdt.month, ptrMember->xmlogdt.day, ptrMember->xmlogdt.year2 );
}
else
{
	printf ( "<td>%s</td>\n", PRINT_NULL_DATE );
}
	printf ( "</tr>\n" );

	return ( 0 );
}

int Lookup ()
{
	int		ndx;
	int		xo, rv;
	char	Fragment[128];

	memset ( WhereClause, '\0', sizeof(WhereClause) );

	for ( ndx = 0, xo = 0; ndx < FieldCount; ndx++ )
	{
		if ( FieldArray[ndx].keyfield )
		{
			continue;
		}

		if ( lastchr ( FieldArray[ndx].buffer, FieldArray[ndx].length ) > 0 )
		{
			if (( nsStrcmp ( FieldArray[ndx].name, "Mrole" ) == 0 ) &&
				( xmember.xmrole[0] == '?' ))
			{
				continue;
			}
			else if ( dbyWildCards ( FieldArray[ndx].buffer ) > 0 )
			{
				sprintf ( Fragment, "%s like '%s'", FieldArray[ndx].name,  FieldArray[ndx].buffer );
			}
			else
			{
				sprintf ( Fragment, "%s = '%s'", FieldArray[ndx].name,  FieldArray[ndx].buffer );
			}

			if ( xo )
			{
				nsStrcat ( WhereClause, " and " );
			}
			nsStrcat ( WhereClause, Fragment );
			xo++;
		}
	}

// printf ( "%s<br>\n", WhereClause );

	rv = LoadMemberCB ( WhereClause, "Mname", &LookupMember, (int(*)()) MyEachMember, 0 );

	if ( rv < 0 )
	{
		SaveError ( "Oops!" );
	}
	else if ( rv == 0 )
	{
		SaveError ( "Nothing matches search criteria entered." );
	}

	return ( rv );

}
