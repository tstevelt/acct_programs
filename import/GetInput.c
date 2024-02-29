/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"import.h"

void GetInput ()
{
	int		xa;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		
		if ( nsStrcmp ( webNames[xa], "ImportAcctnum" ) == 0 )
		{
			ImportAcctnum = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "import_filename" ) == 0 )
		{
			snprintf ( import_filename, sizeof(import_filename), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "submitImport" ) == 0 )
		{
			RunMode = MODE_IMPORT;
		}
		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}
	}
}

