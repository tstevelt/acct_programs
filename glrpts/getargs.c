				/*
				acct_programs/rptall
				acct_programs/rptcf
				acct_programs/rptcoa
				acct_programs/rptgl
				acct_programs/rpttax
				acct_programs/rpttrx
				*/

#include	"glrpts.h"

static void Usage ()
{
	printf ( "USAGE: report args\n" );
	printf ( "  coa\n" );
	printf ( "  bs date\n" );
	printf ( "  is start end\n" );
	printf ( "  cf start end\n" );
	printf ( "  tax args\n" );
	printf ( "  trx start end title\n" );
	printf ( "  all start end\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	if ( argc < 2 )
	{
		Usage ();
	}

	RunMode = 0;
	if ( strcmp ( argv[1], "coa" ) == 0 )
	{
		RunMode = MODE_COA;
	}
	else if ( strcmp ( argv[1], "bs" ) == 0 )
	{
		RunMode = MODE_BS;
	}
	else if ( strcmp ( argv[1], "is" ) == 0 )
	{
		RunMode = MODE_IS;
	}
	else if ( strcmp ( argv[1], "cf" ) == 0 )
	{
		RunMode = MODE_CF;
	}
	else if ( strcmp ( argv[1], "tax" ) == 0 )
	{
		RunMode = MODE_TAX;
	}
	else if ( strcmp ( argv[1], "trx" ) == 0 )
	{
		RunMode = MODE_TRX;
	}
	else if ( strcmp ( argv[1], "all" ) == 0 )
	{
		RunMode = MODE_ALL;
	}
	else
	{
		Usage ();
	}
	
}
