/*----------------------------------------------------------------------------
	Program : ChangeAcctnum.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update account number in all affected tables
	Return  : 
----------------------------------------------------------------------------*/

#include	"chgacct.h"

typedef struct
{
	char	*Table;
	char	*Field;
} RECORD;

static	RECORD	Array[] =
{
	{ "account","acctnum" },
	{ "import",	"acctnum" },
	{ "rptacct","acctnum" },
	{ "trxd",	"acctnum" },
};

static	int		Count = sizeof(Array) / sizeof(RECORD);

void ChangeAcctnum ()
{
	int		ndx;
	long	RecCount, Affected;

	printf ( "<table class='AppWide'0%%'>\n" );
	printf ( "<tr><td colspan='3'>" );
	printf ( "Change %ld to %ld", CurrentAcctnum, NewAcctnum );
	printf ( "</td></tr>" );
	for ( ndx = 0; ndx < Count; ndx++ )
	{
		sprintf ( StatementOne, "%s = %ld", Array[ndx].Field, CurrentAcctnum );
		RecCount = dbySelectCount ( &MySql, Array[ndx].Table, StatementOne, LOGFILENAME );

		printf ( "<tr><td colspan='3'>" );
		printf ( "%ld in %s", RecCount, Array[ndx].Table );

		if ( RecCount > 0 )
		{
			sprintf ( StatementOne, "update %s set %s = %ld where %s = %ld", 
					Array[ndx].Table, Array[ndx].Field, NewAcctnum, Array[ndx].Field, CurrentAcctnum );
			Affected = dbyUpdate ( "chgacct", &MySql, StatementOne, 0, LOGFILENAME );
			if ( Affected != RecCount )
			{
				printf ( "</td></tr>" );
				printf ( "<tr><td colspan='3'>" );
				printf ( "ERROR: Updated %ld of %ld in %s", Affected, RecCount, Array[ndx].Table );
				printf ( "</td></tr>" );
				printf ( "<tr><td colspan='3'>" );
				printf ( "%s", StatementOne );
				printf ( "</td></tr>" );
				return;
			}
			else
			{
				printf ( " -- Okay" );
			}
		}

		printf ( "</td></tr>" );
	}
	printf ( "</table>\n" );
}
