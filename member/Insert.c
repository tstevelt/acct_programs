/*----------------------------------------------------------------------------
	Program : Insert.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert member record.
	Return  : 
----------------------------------------------------------------------------*/

#include	"member.h"

void Insert ()
{
	if ( xmember.xid > 0 )
	{
		sprintf ( StatementOne, "insert into member (id, Mname, Memail, Mphone, Mcarrier, Mtwopref, Mrole, Mpassword, Minsdt ) values (%ld, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%04d-%02d-%02d' )",
xmember.xid
, xmember.xmname
, xmember.xmemail
, xmember.xmphone
, xmember.xmcarrier
, xmember.xmtwopref
, xmember.xmrole
, pw_sha_make_pw ( (unsigned char *) xmember.xmpassword )
, xmember.xminsdt.year4, xmember.xminsdt.month, xmember.xminsdt.day );
	}
	else
	{
		sprintf ( StatementOne, "insert into member (Mname, Memail, Mphone, Mcarrier, Mtwopref, Mrole, Mpassword, Minsdt ) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%04d-%02d-%02d' )",
xmember.xmname
, xmember.xmemail
, xmember.xmphone
, xmember.xmcarrier
, xmember.xmtwopref
, xmember.xmrole
, pw_sha_make_pw ( (unsigned char *) xmember.xmpassword )
, xmember.xminsdt.year4, xmember.xminsdt.month, xmember.xminsdt.day );
	}

	if ( dbyInsert ( "member", &MySql, StatementOne, 0, LOGFILENAME ) != 1 )
	{
		SaveError ( "dbyInsert failed" );
		RunMode = MODE_START;
		return;
	}

	if (( xmember.xid = mysql_insert_id ( &MySql )) == 0L )
	{
		SaveError ( "dbyInsert failed to insert new memeber" );
		RunMode = MODE_START;
		return;
	}
	else
	{
		snprintf ( WhereClause, sizeof(WhereClause), "id = %ld", xmember.xid );
		LoadMember ( WhereClause, &xmember, 0 );
	}
}
