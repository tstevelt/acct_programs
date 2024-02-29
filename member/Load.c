/*----------------------------------------------------------------------------
	Program : Load.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load member record
	Return  : 
----------------------------------------------------------------------------*/

#include	"member.h"

static	int		DebugLoad = 0;

int Load ()
{
	int		rv = 0;

	RunMode = MODE_FOUND;

	snprintf ( WhereClause, sizeof(WhereClause), "member.id = %ld", xmember.xid );
	if (( rv = LoadMember ( WhereClause, &xmember, 0 )) != 1 )
	{
		if ( DebugLoad )
		{
			sprintf ( msgbuf, "Unknown ID %ld, rv = %d", xmember.xid, rv );
			SaveError ( msgbuf );
		}
		else
		{
			SaveError ( "Unknown ID" );
		}
		RunMode = MODE_START;
	}

	return ( rv );
}
