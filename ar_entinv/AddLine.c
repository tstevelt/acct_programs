/*----------------------------------------------------------------------------
	Program : AddLine.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert one invoice detail record
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_entinv.h"

void AddLine ()
{
	sprintf ( StatementOne, 
		"insert into arinvl ( invoice, descr ) values ( %ld, ' ' )",
			xarinvh.xid );

	if ( dbyInsert ( "ar_entinv", &MySql, StatementOne, 0, LOGFILENAME ) !=  1 )
	{
		SaveError ( "Add line failed." );
		return;
	}
}
