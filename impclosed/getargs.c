
#include	"impclosed.h"

static void Usage ()
{
	fprintf ( stderr, "USAGE: impclosed db asset shortterm longterm file [-d]\n" );
	exit ( 1 );
}

void getargs ( int argc, char *argv[] )
{
	if ( argc < 5 )
	{
		Usage ();
	}

	Database = argv[1];

	AssetAccount = atol(argv[2]);

	ShortTermAccount = atol(argv[3]);

	LongTermAccount = atol(argv[4]);

	Filename = argv[5];

	for ( int xa = 6; xa < argc; xa++ )
	{
		if ( strcmp ( argv[xa], "-d" ) == 0 )
		{
			Verbose = 1;
		}
		else
		{
			fprintf ( stderr, "Unknown argument %s\n", argv[xa] );
			exit ( 1 );
		}
	}

}
