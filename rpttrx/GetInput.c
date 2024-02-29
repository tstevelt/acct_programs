/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/


#include	"rpttrx.h"

void GetInput ()
{
	int		xa;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;
	ReportFormat =  RPT_FORMAT_PDF_VIEW;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "type" ) == 0 )
		{
			CategoryType = toupper ( webValues[xa][0] );
		}
		else if ( nsStrcmp ( webNames[xa], "title" ) == 0 )
		{
			snprintf ( xrpt.xtitle, sizeof(xrpt.xtitle), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "rptname" ) == 0 )
		{
			snprintf ( xrpt.xrptname, sizeof(xrpt.xrptname), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "startdate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xrpt.xstartdate );
		}
		else if ( nsStrcmp ( webNames[xa], "enddate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xrpt.xenddate );
		}
		else if ( nsStrcmp ( webNames[xa], "ReportFormat" ) == 0 )
		{
			ReportFormat = toupper ( webValues[xa][0] );
		}
		else if ( nsStrncmp ( webNames[xa], "cb_", 3 ) == 0 )
		{
			if ( Count < MAXCATEGORY )
			{
				Array[Count] = nsAtoi ( &webNames[xa][3] );
			}
			else if ( Count == MAXCATEGORY )
			{
				SaveError ( "Exceeds MAXCATEGORY" );
			}
			Count++;
		}
		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "save" ) == 0 )
			{
				RunMode = MODE_SAVE;
			}
			else if ( nsStrcmp ( webValues[xa], "find" ) == 0 )
			{
				RunMode = MODE_FIND;
			}
			else if ( nsStrcmp ( webValues[xa], "go" ) == 0 )
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
