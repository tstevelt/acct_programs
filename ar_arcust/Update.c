/*----------------------------------------------------------------------------
	Program : Update.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update customer record
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

void Update ()
{
	time_t	TimeStamp;
	int		Affected;

	TimeStamp = time ( NULL );

	if ( xarcust.xid > 0 )
	{
		sprintf ( StatementOne, 
		"update arcust set name = '%s', addr1 = '%s', addr2 = '%s', city = '%s', state = '%s', zipcode = '%s', phone = '%s', contact = '%s' where id = %ld",
			xarcust.xname,
			xarcust.xaddr1,
			xarcust.xaddr2,
			xarcust.xcity,
			xarcust.xstate,
			xarcust.xzipcode,
			xarcust.xphone,
			xarcust.xcontact,
			xarcust.xid );
	}

	Affected = dbyUpdate ( "ar_arcust", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( Affected < 0 )
	{
		SaveError ( "dbyUpdate failed" );
		RunMode = MODE_FOUND;
		return;
	}
	else if ( Affected == 0 )
	{
		SaveError ( "No changes" );
		RunMode = MODE_FOUND;
		return;
	}
	else
	{
		sprintf ( StatementOne, "update arcust set updated = %ld where id = %ld", TimeStamp, xarcust.xid );
		dbyUpdate ( "ar_arcust", &MySql, StatementOne, 0, LOGFILENAME );
	}

	snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xarcust.xid );
	LoadArcust ( WhereClause, &xarcust, 0 );
}
