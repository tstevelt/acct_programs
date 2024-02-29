/*----------------------------------------------------------------------------
	Program : Delete.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Delete member record.
	Return  : 
----------------------------------------------------------------------------*/

#include	"member.h"

int Delete ()
{
	int		Affected;

	if ( xmember.xid > 0 )
	{
		sprintf ( StatementOne, "delete from member where id = %ld", xmember.xid );
	}
	else
	{
		return ( -1 );
	}

	Affected = dbyDelete ( "member", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( Affected < 0 )
	{
		SaveError ( "dbyDelete failed" );
		RunMode = MODE_FOUND;
		return ( -2 );
	}
	else if ( Affected == 0 )
	{
		SaveError ( "Customer not deleted" );
		RunMode = MODE_FOUND;
		return ( -3 );
	}
	else
	{
		RunMode = MODE_START;
	}

	return ( 0 );
}
