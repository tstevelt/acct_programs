/*----------------------------------------------------------------------------
	Program : Lookup.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint customer lookup screen
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

#include	"ar_arcust.h"

static XARCUST LookupCust;

#define SCARY_SALESMAN

static int MyEachArcust ( XARCUST *ptrArcust )
{
	static int firstpass = 1;

	if ( firstpass )
	{
		firstpass = 0;

		printf ( "<table class='AppWide'>\n" );
		printf ( "<tr>\n" );
		printf ( "<td>id</td>\n" );
		printf ( "<td>name</td>\n" );
#ifdef SCARY_SALESMAN
		printf ( "<td>contact</td>\n" );
		printf ( "<td>phone</td>\n" );
#endif
		printf ( "</tr>\n" );
	}

	printf ( "<tr>\n" );
	printf ( "<td>" );
	printf ( "<a href='ar_arcust.cgi?submitLoad=load&field_id=%ld'>%ld</a>", ptrArcust->xid, ptrArcust->xid );
	printf ( "</td>\n" );
	printf ( "<td>%s</td>\n", ptrArcust->xname );
#ifdef SCARY_SALESMAN
	printf ( "<td>%s</td>\n", ptrArcust->xcontact );
	printf ( "<td>%s</td>\n", ptrArcust->xphone );
#endif
	printf ( "</tr>\n" );

	return ( 0 );
}

int Lookup ()
{
	int		ndx, rv;
	int		xo;
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
			if ( dbyWildCards ( FieldArray[ndx].buffer ) > 0 )
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

	rv = LoadArcustCB ( WhereClause, "name", &LookupCust, (int(*)()) MyEachArcust, 1 );

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
