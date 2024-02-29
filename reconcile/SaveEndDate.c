/*----------------------------------------------------------------------------
	Program : SaveEndDate.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update account record recon date, balance
	Return  : 
----------------------------------------------------------------------------*/

#include	"reconcile.h"

void SaveEndDate ()
{
	if ( EndDate.month == 0 )
	{
		return;
	}

	sprintf ( StatementOne,
"update account set recondate = '%04d-%02d-%02d', reconbal = %ld where acctnum = %ld and recondate < '%04d-%02d-%02d'",
				EndDate.year4, EndDate.month, EndDate.day, 
				EndBalance,
				xaccount.xacctnum,
				EndDate.year4, EndDate.month, EndDate.day );

//printf ( "%s<br>\n", StatementOne );

	dbyUpdate ( "reconcile", &MySql, StatementOne, 0, LOGFILENAME );

}
