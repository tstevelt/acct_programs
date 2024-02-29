/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_statement.h"

void GetInput ()
{
	int		xa;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;
	ReportFormat =  RPT_FORMAT_HTML;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "ReportFormat" ) == 0 )
		{
			ReportFormat = toupper ( webValues[xa][0] );
		}
		else if ( nsStrcmp ( webNames[xa], "CustomerNumber" ) == 0 )
		{
			CustomerNumber = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "IncludeInstructions" ) == 0 )
		{
			IncludeInstructions = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "go" ) == 0 )
			{
				RunMode = MODE_RUN;
			}
			else if ( nsStrcmp ( webValues[xa], "lunch" ) != 0 )
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
