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

#include	"asetup.h"

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

		if ( nsStrcmp ( webNames[xa], "name" ) == 0 )
		{
			snprintf ( xsystem.xname, sizeof( xsystem.xname), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "address" ) == 0 )
		{
			snprintf ( xsystem.xaddress, sizeof( xsystem.xaddress), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "city" ) == 0 )
		{
			snprintf ( xsystem.xcity, sizeof( xsystem.xcity), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "state" ) == 0 )
		{
			snprintf ( xsystem.xstate, sizeof( xsystem.xstate), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "zipcode" ) == 0 )
		{
			snprintf ( xsystem.xzipcode, sizeof( xsystem.xzipcode), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "phone" ) == 0 )
		{
			snprintf ( xsystem.xphone, sizeof( xsystem.xphone), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "twofactor" ) == 0 )
		{
			xsystem.xtwofactor[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "assetnext" ) == 0 )
		{
			xsystem.xassetnext = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "liabnext" ) == 0 )
		{
			xsystem.xliabnext = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "equitynext" ) == 0 )
		{
			xsystem.xequitynext = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "incomenext" ) == 0 )
		{
			xsystem.xincomenext = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "expensenext" ) == 0 )
		{
			xsystem.xexpensenext = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "showamts" ) == 0 )
		{
			xsystem.xshowamts[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "printdest" ) == 0 )
		{
			xsystem.xprintdest[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "reconcile" ) == 0 )
		{
			xsystem.xreconcile[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "numtrxnew" ) == 0 )
		{
			xsystem.xnumtrxnew = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "numtrxedit" ) == 0 )
		{
			xsystem.xnumtrxedit = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "closemethod" ) == 0 )
		{
			xsystem.xclosemethod[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "retainequity" ) == 0 )
		{
			xsystem.xretainequity = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "retainincome" ) == 0 )
		{
			xsystem.xretainincome = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "page" ) == 0 )
		{
			PageNumber = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "save" ) == 0 )
			{
				RunMode = MODE_SAVE;
			}
		}

		else
		{
			sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
			SaveError ( StatementOne );
		}

	}
	
}
