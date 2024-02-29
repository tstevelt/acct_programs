/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
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

#include	"chpw.h"


void GetInput ()
{
	int		xa;
	char	PlainText[256];
	char	*tokens[4];
	int		tokcnt;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_PAINT_CHANGE;

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

// [forgot] [demo_demo_1608072914

		if ( nsStrcmp ( webNames[xa], "forgot" ) == 0 )
		{
			sprintf ( PlainText, "%s", tmsDecode(webValues[xa]) );

			if (( tokcnt = GetTokensStd ( PlainText, "_", tokens, 4, 0 )) < 3 )
			{
				sprintf ( StatementOne, "forgot string syntax error" );
				SaveError ( StatementOne );
			}
			else
			{
				sprintf ( Database, "%s", tokens[0] );
				sprintf ( MemberName, "%s", tokens[1] );
				ExpireTime = nsAtol ( tokens[2] );
				RunMode = MODE_PAINT_FORGOT;
			}
			
		}

		else if ( nsStrcmp ( webNames[xa], "Database" ) == 0 )
		{
			sprintf ( Database, "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "MemberName" ) == 0 )
		{
			sprintf ( MemberName, "%s", webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "CurrentPassword" ) == 0 )
		{
			CurrentPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "NewPassword" ) == 0 )
		{
			NewPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "ConfirmPassword" ) == 0 )
		{
			ConfirmPassword = webValues[xa];
		}
		else if ( nsStrcmp ( webNames[xa], "submitSaveChange" ) == 0 )
		{
			RunMode = MODE_SAVE_CHANGE;
		}
		else if ( nsStrcmp ( webNames[xa], "submitSaveForgot" ) == 0 )
		{
			RunMode = MODE_SAVE_FORGOT;
		}

		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}

	}
	
}
