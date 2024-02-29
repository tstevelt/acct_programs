/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_arcust.h"

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
			xarcust.xid = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_name" ) == 0 )
		{
			snprintf ( xarcust.xname, sizeof(xarcust.xname), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_addr1" ) == 0 )
		{
			snprintf ( xarcust.xaddr1, sizeof(xarcust.xaddr1), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_addr2" ) == 0 )
		{
			snprintf ( xarcust.xaddr2, sizeof(xarcust.xaddr2), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_city" ) == 0 )
		{
			snprintf ( xarcust.xcity, sizeof(xarcust.xcity), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_state" ) == 0 )
		{
			snprintf ( xarcust.xstate, sizeof(xarcust.xstate), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_zipcode" ) == 0 )
		{
			snprintf ( xarcust.xzipcode, sizeof(xarcust.xzipcode), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_phone" ) == 0 )
		{
			snprintf ( xarcust.xphone, sizeof(xarcust.xphone), "%-s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "field_contact" ) == 0 )
		{
			snprintf ( xarcust.xcontact, sizeof(xarcust.xcontact), "%-s", webValues[xa] );
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
