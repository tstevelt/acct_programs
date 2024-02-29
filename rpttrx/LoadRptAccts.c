/*----------------------------------------------------------------------------
	Program : LoadRptAccts.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Load saved reports
	Return  : 
----------------------------------------------------------------------------*/

#include	"rpttrx.h"

void LoadRptAccts ( char RptName[] )
{
	DBY_QUERY	*Query;

	sprintf ( StatementOne, 
		"select %s from rptacct where rptname = '%s' order by seqnum",
			FIELDS_RPTACCT, RptName );

	Query = dbySelect ( "rpttrx", &MySql, StatementOne, LOGFILENAME );

	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		if ( Count >= MAXCATEGORY )
		{
			SaveError ( "too many categories" );
			return;
		}
		Array[Count] = safe_atoi ( Query->EachRow[RPTACCT_acctnum] );
		Count++;
	}
}
