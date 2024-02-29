/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"chgacct.h"

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
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "CurrentAcctnum" ) == 0 )
		{
			CurrentAcctnum = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "NewAcctnum" ) == 0 )
		{
			NewAcctnum = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "check" ) == 0 )
			{
				RunMode = MODE_CHECK;
			}
			else if ( nsStrcmp ( webValues[xa], "save" ) == 0 )
			{
				RunMode = MODE_SAVE;
			}
			else if ( nsStrcmp ( webValues[xa], "start" ) == 0 )
			{
				RunMode = MODE_START;
			}
			else
			{
				sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
				SaveError ( StatementOne );
			}
		}

		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}
	}
}
