/*----------------------------------------------------------------------------
	Program : SaveImport.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update import record
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

int SaveImport ( int AccountNumber )
{
	int		rv = 0;

#ifdef VERSION_ONE
	sprintf ( StatementOne, 
"update import set website = '%s', login = '%s', previous = '%04d-%02d-%02d', payments = '%c', autoparse = '%c' where acctnum = %d",
			ximport.xwebsite,
			ximport.xlogin,
			ximport.xprevious.year4, ximport.xprevious.month, ximport.xprevious.day,
			ximport.xpayments[0],
			ximport.xautoparse[0],
			AccountNumber );
#else
	sprintf ( StatementOne, 
"update import set website = '%s', login = '%s', previous = '%04d-%02d-%02d', payments = '%c', reverse = '%c' where acctnum = %d",
			ximport.xwebsite,
			ximport.xlogin,
			ximport.xprevious.year4, ximport.xprevious.month, ximport.xprevious.day,
			ximport.xpayments[0],
			ximport.xreverse[0],
			AccountNumber );
#endif
	
// printf ( "%s<br>\n", StatementOne );
	rv = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	
#ifdef VERSION_ONE
	if ( ximport.xautoparse[0] == 'Y' )
	{
		sprintf ( StatementOne, 
"update import set headrows = %d, colcount = %d, refnum = %d, date = %d, payee = %d, increase = %d, incsign = '%c', decrease = %d, decsign = '%c' where acctnum = %d",
			ximport.xheadrows,
			ximport.xcolcount,
			ximport.xrefnum,
			ximport.xdate,
			ximport.xpayee,
			ximport.xincrease,
			ximport.xincsign[0],
			ximport.xdecrease,
			ximport.xdecsign[0],
			AccountNumber );
	}
	else
	{
		sprintf ( StatementOne, 
			"update import set parsefunc = '%s' where acctnum = %d",
				ximport.xparsefunc,
				AccountNumber );
	}

// printf ( "%s<br>\n", StatementOne );
	rv = dbyUpdate ( "import", &MySql, StatementOne, 0, LOGFILENAME );
#endif

	return ( rv );
}
