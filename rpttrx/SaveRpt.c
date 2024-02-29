/*----------------------------------------------------------------------------
	Program : SaveRpt.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Save report setup
	Return  : 
----------------------------------------------------------------------------*/

#include	"rpttrx.h"

int SaveRpt ( char RptName[], char CategoryType )
{
	int		rv = 0;
	XRPT	NewRpt;
	int		Affected;
	int		xc;

// printf ( "%s<br>\n", RptName );

	memcpy ( &NewRpt, &xrpt, sizeof(XRPT) );
	NewRpt.xaccttype[0] = CategoryType;

	if ( LoadOneRpt ( RptName, CategoryType ) == 1 )
	{
		if ( memcmp ( &NewRpt, &xrpt, sizeof(XRPT) ) != 0 )
		{
			sprintf ( StatementOne, 
"update rpt set startdate = '%04d-%02d-%02d', enddate = '%04d-%02d-%02d', title = '%s' where rptname = '%s' and accttype = '%c'",
				NewRpt.xstartdate.year4,
				NewRpt.xstartdate.month,
				NewRpt.xstartdate.day,
				NewRpt.xenddate.year4,
				NewRpt.xenddate.month,
				NewRpt.xenddate.day,
				NewRpt.xtitle,
				RptName,
				CategoryType );

// printf ( "%s<br>\n", StatementOne );

			Affected = dbyUpdate ( "rpttrx", &MySql, StatementOne, 0, LOGFILENAME );

			if ( Affected > 0 )
			{
				rv = 1;
			}
		}

		sprintf ( StatementOne,
			"delete from rptacct where rptname = '%s'", RptName );

		Affected = dbyDelete ( "rpttrx", &MySql, StatementOne, 0, LOGFILENAME );
	}
	else
	{
		sprintf ( StatementOne, 
"insert into rpt (rptname, startdate, enddate, title, accttype ) \
  values ( '%s', '%04d-%02d-%02d', '%04d-%02d-%02d', '%s', '%c' )",
			RptName,
			NewRpt.xstartdate.year4,
			NewRpt.xstartdate.month,
			NewRpt.xstartdate.day,
			NewRpt.xenddate.year4,
			NewRpt.xenddate.month,
			NewRpt.xenddate.day,
			NewRpt.xtitle,
			CategoryType );
			
// printf ( "%s<br>\n", StatementOne );

		Affected = dbyInsert ( "rpttrx", &MySql, StatementOne, 0, LOGFILENAME );

		if ( Affected > 0 )
		{
			rv = 2;
		}
	}

	for ( xc = 0; xc < Count; xc++ )
	{
		sprintf ( StatementOne, 
			"insert into rptacct (rptname, seqnum, acctnum ) values ( '%s', '%d', '%d' )",
				RptName, xc, Array[xc] );

		Affected = dbyInsert ( "rpttrx", &MySql, StatementOne, 0, LOGFILENAME );

	}

	return ( rv );
}
