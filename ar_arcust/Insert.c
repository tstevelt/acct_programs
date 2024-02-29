/*----------------------------------------------------------------------------
	Program : Insert.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert a customer record.
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

void Insert ()
{
	if ( xarcust.xid > 0 )
	{
		sprintf ( StatementOne, 
	"insert into arcust (id, name, addr1, addr2, city, state, zipcode, phone, contact ) values ( %ld, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )",
			xarcust.xid,
			xarcust.xname,
			xarcust.xaddr1,
			xarcust.xaddr2,
			xarcust.xcity,
			xarcust.xstate,
			xarcust.xzipcode,
			xarcust.xphone,
			xarcust.xcontact );

	}
	else
	{
		sprintf ( StatementOne, 
	"insert into arcust ( name, addr1, addr2, city, state, zipcode, phone, contact ) values ( '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )",
			xarcust.xname,
			xarcust.xaddr1,
			xarcust.xaddr2,
			xarcust.xcity,
			xarcust.xstate,
			xarcust.xzipcode,
			xarcust.xphone,
			xarcust.xcontact );
	}

	if ( dbyInsert ( "ar_arcust", &MySql, StatementOne, 0, LOGFILENAME ) != 1 )
	{
		SaveError ( "dbyInsert failed" );
		RunMode = MODE_START;
		return;
	}

	if (( xarcust.xid = mysql_insert_id ( &MySql )) == 0 )
	{
		SaveError ( "dbyInsert failed to insert new customer" );
		RunMode = MODE_START;
		return;
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xarcust.xid );
		LoadArcust ( WhereClause, &xarcust, 0 );
	}
}
