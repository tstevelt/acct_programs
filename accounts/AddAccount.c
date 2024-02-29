/*----------------------------------------------------------------------------
	Program : AddAccount.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert account record
	Return  : 
----------------------------------------------------------------------------*/
//     Accounting Programs
// 
//     Copyright (C)  2000-2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include	"accounts.h"

int AddAccount ( void )
{
	int		rv = 0;
	char	*fldname;
	int		MyCount;
	long	TimeStamp;

	TimeStamp = (long) time(NULL);

	MyCount = 0;
	if ( xaccount.xaccttype[0] == '?' )
	{
		SaveError ( "no account type selected." );
		MyCount++;
	}
	if ( xaccount.xacctnum <= 0L )
	{
		SaveError ( "can not add account without an account number." );
		MyCount++;
	}
	if ( xaccount.xacctname[0] == '\0' )
	{
		SaveError ( "can not add account without a name." );
		MyCount++;
	}
	if ( MyCount > 0 )
	{
		return ( rv );
	}

	xaccount.xcurrbal = xaccount.xopenbal;

	sprintf ( StatementOne, 
			"insert into account (%s) values ('%ld','%s','%s','%s','%s',%ld,%ld,'%4d-%02d-%02d',%ld,%d,%ld,%ld)",
				FIELDS_ACCOUNT,
				xaccount.xacctnum,
				xaccount.xaccttype,
				xaccount.xacctname,
				xaccount.xextranum,
				xaccount.xextraline,
				xaccount.xopenbal,
				xaccount.xcurrbal,
				xaccount.xrecondate.year4, xaccount.xrecondate.month, xaccount.xrecondate.day,
				xaccount.xreconbal,
				xaccount.xdaterange, TimeStamp, 0L );
	
// printf ( "%s<br>\n", StatementOne );

	rv = dbyInsert  ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	
	if ( rv == 1 )
	{

#ifdef THIS_RETURNS_ZERO_SINCE_FIELD_IS_NOT_AUTO_INC
		sprintf ( msgbuf, "mysql_insert_id returned %lld.", mysql_insert_id(&MySql) );
		SaveError ( msgbuf );
#endif	

		switch ( xaccount.xaccttype[0] )
		{
			case 'A': fldname = "assetnext"; break;
			case 'L': fldname = "liabnext"; break;
			case 'E': fldname = "equitynext"; break;
			case 'I': fldname = "incomenext"; break;
			case 'X': fldname = "expensenext"; break;
			case 'T': fldname = "trxnext"; break;
		}

		sprintf ( StatementOne, 
				"update system set %s = %ld where %s = %ld",
					fldname, xaccount.xacctnum + 1, fldname, xaccount.xacctnum );

		dbyUpdate  ( "import", &MySql, StatementOne, 0, LOGFILENAME );
	}
	else
	{
		SaveError ( "an error occurred, account not added." );
	}

	return ( rv );
}
