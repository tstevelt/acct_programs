/*----------------------------------------------------------------------------
	Program : PaintUsers.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint users list.
	Return  : 
----------------------------------------------------------------------------*/

#include	"asetup.h"

int PaintUsers ()
{
	DBY_QUERY		*Query;
	int				Rows = 0;

	sprintf ( StatementOne, "select %s from member order by Mname", FIELDS_MEMBER );

	Query = dbySelect ( "setup", &MySql, StatementOne, LOGFILENAME );

	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		Rows++;

		if ( Rows == 1 )
		{
			printf ( "<tr>\n" );
			printf ( "<th>id</th>\n" );
			printf ( "<th>name</th>\n" );
			printf ( "<th>last login</th>\n" );
			printf ( "</tr>\n" );
		}

		printf ( "<tr>\n" );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_id] );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_Mname] );
		printf ( "<td>%s</td>\n", Query->EachRow[MEMBER_Mlogdt] );
		printf ( "</tr>\n" );


	}

	return ( Rows );
}
