/*----------------------------------------------------------------------------
	Program : glrpts.c
	Author  : Tom Stevelt
	Date    : 07/21/2024
	Synopsis: framework to create all GL reports. 
	Call By : called by these programs. Can be run from command line.
				acct_programs/rptall
				acct_programs/rptcf
				acct_programs/rptcoa
				acct_programs/rptgl
				acct_programs/rpttax
				acct_programs/rpttrx

	Who		Date		Modification
	---------------------------------------------------------------------

----------------------------------------------------------------------------*/

#define MAIN
#include	"glrpts.h"

int	UseErrorSys = 1;

int main ( int argc, char *argv[] )
{
	int		rv;

	getargs ( argc, argv );
	
	// AcctGetCookieAndSession ( &SessionRecord );

#ifdef JUNK
Program : StartDb.c
static	int		DebugStartDb = 1;
int StartDb ( char *DbName )
	if ( DebugStartDb )
	rv = StartDb ( "acct" );
	printf ( "StartDb: dbyConnect returned %d<br>\n", rv );
/usr/local/include/dbylib.h:/* dbyConnect.c */
/usr/local/include/dbylib.h:char *dbyConnectErrorStr ( int ErrorCode );
/usr/local/include/dbylib.h:int dbyConnect ( MYSQL *MySql , char *DbName , char *UserName , int CheckStatus , FILE *fpError );
#endif
	rv = dbyConnect ( &MySql, "acct", "tms", 1, stderr );
	printf ( "dbyConnect returned %d<br>\n", rv );

	switch ( RunMode )
	{
		case MODE_COA:
			if ( getdata_coa () == 0 )
			{
				dorpt_coa ();
			}
			break;
		case MODE_BS:
		case MODE_IS:
		case MODE_CF:
		case MODE_TAX:
		case MODE_TRX:
		case MODE_ALL:
			printf ( "Mode %d not finished\n", RunMode );
			exit ( 1 );
		default:
			printf ( "Unknown Mode %d\n", RunMode );
			exit ( 1 );
	}

	dbyClose ( &MySql );

	return ( 0 );
}
