/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"member.h"

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

		if ( nsStrcmp ( webNames[xa], "field_id" ) == 0 )
		{
			xmember.xid = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mname" ) == 0 )
		{
			sprintf ( xmember.xmname, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_memail" ) == 0 )
		{
			sprintf ( xmember.xmemail, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mphone" ) == 0 )
		{
			sprintf ( xmember.xmphone, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mcarrier" ) == 0 )
		{
			sprintf ( xmember.xmcarrier, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mtwopref" ) == 0 )
		{
			sprintf ( xmember.xmtwopref, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mrole" ) == 0 )
		{
			sprintf ( xmember.xmrole, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mipaddr" ) == 0 )
		{
			sprintf ( xmember.xmipaddr, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mpassword" ) == 0 )
		{
			sprintf ( xmember.xmpassword, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_minsdt" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xmember.xminsdt );
		}
		else if ( nsStrcmp ( webNames[xa], "field_mlogdt" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xmember.xmlogdt );
		}
		else if ( nsStrcmp ( webNames[xa], "field_inserted" ) == 0 )
		{
			xmember.xinserted = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_updated" ) == 0 )
		{
			xmember.xupdated = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "submitLoad" ) == 0 )
		{
			RunMode = MODE_LOAD;
		}
		else if ( nsStrcmp ( webNames[xa], "submitLookup" ) == 0 )
		{
			RunMode = MODE_LOOKUP;
		}
		else if ( nsStrcmp ( webNames[xa], "submitInsert" ) == 0 )
		{
			RunMode = MODE_INSERT;
		}
		else if ( nsStrcmp ( webNames[xa], "submitUpdate" ) == 0 )
		{
			RunMode = MODE_UPDATE;
		}

#ifdef SAFETY_ON_DELETE
		else if ( nsStrcmp ( webNames[xa], "HiddenSafety" ) == 0 )
		{
			HiddenSafety = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "AnswerSafety" ) == 0 )
		{
			AnswerSafety = nsAtoi ( webValues[xa] );
		}
#endif
		else if ( nsStrcmp ( webNames[xa], "submitDelete" ) == 0 )
		{
			RunMode = MODE_DELETE;
		}
		else if ( nsStrcmp ( webNames[xa], "submitQuit" ) == 0 )
		{
			RunMode = MODE_QUIT;
		}
		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}

	}
	
}
