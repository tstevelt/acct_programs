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

#include	"accounts.h"

void GetInput ()
{
	int		xa;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput ();

	RunMode = MODE_START;
	AC_Mode = '?';

	ximport.xpayments[0] = 'N';
	ximport.xreverse[0] = 'N';
#ifdef VERSION_ONE
	ximport.xautoparse[0] = 'N';
	ximport.xincsign[0] = 'N';
	ximport.xdecsign[0] = 'N';
#endif

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

// printf ( "%s %s<br>\n", webNames[xa], webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "ac" ) == 0 )
		{
			AC_Mode = toupper ( webValues[xa][0] );
		}
		else if ( nsStrcmp ( webNames[xa], "add" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "account" ) == 0 )
			{
				RunMode = MODE_ADD_ACCOUNT;
			}
			else if ( nsStrcmp ( webValues[xa], "import" ) == 0 )
			{
				RunMode = MODE_ADD_IMPORT;
			}
			else
			{
				sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
				SaveError ( StatementOne );
			}
		}
		else if ( nsStrcmp ( webNames[xa], "edit" ) == 0 )
		{
			RunMode = MODE_EDIT;
			xaccount.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "register" ) == 0 )
		{
			RunMode = MODE_REGISTER;
			xaccount.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "list" ) == 0 )
		{
			RunMode = MODE_START;
		}
		else if ( nsStrcmp ( webNames[xa], "quit" ) == 0 )
		{
			RunMode = MODE_QUIT;
		}
		
		else if ( nsStrcmp ( webNames[xa], "accttype" ) == 0 )
		{
			xaccount.xaccttype[0] = webValues[xa][0];
		}
		else if ( nsStrcmp ( webNames[xa], "account_acctnum" ) == 0 )
		{
			xaccount.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "import_acctnum" ) == 0 )
		{
			ximport.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "acctname" ) == 0 )
		{
			snprintf ( xaccount.xacctname, sizeof(xaccount.xacctname), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "extranum" ) == 0 )
		{
			snprintf ( xaccount.xextranum, sizeof(xaccount.xextranum), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "extraline" ) == 0 )
		{
			snprintf ( xaccount.xextraline, sizeof(xaccount.xextraline), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "openbal" ) == 0 )
		{
			xaccount.xopenbal = nsAtof ( webValues[xa] ) * 100.0 + 0.5;
		}
		else if ( nsStrcmp ( webNames[xa], "recondate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xaccount.xrecondate );
		}
		else if ( nsStrcmp ( webNames[xa], "reconbal" ) == 0 )
		{
			xaccount.xreconbal = nsAtof ( webValues[xa] ) * 100.0 + 0.5;
		}
		else if ( nsStrcmp ( webNames[xa], "daterange" ) == 0 )
		{
			xaccount.xdaterange = nsAtoi ( webValues[xa] );
		}

		/*--------------------------------------------------------------
			import
		--------------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "payments" ) == 0 )
		{
			ximport.xpayments[0] = 'Y';
		}
		else if ( nsStrcmp ( webNames[xa], "reverse" ) == 0 )
		{
			ximport.xreverse[0] = 'Y';
		}
		else if ( nsStrcmp ( webNames[xa], "website" ) == 0 )
		{
			snprintf ( ximport.xwebsite, sizeof(ximport.xwebsite), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "login" ) == 0 )
		{
			snprintf ( ximport.xlogin, sizeof(ximport.xlogin), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "previous" ) == 0 )
		{
			StringToDateval ( webValues[xa], &ximport.xprevious );
		}
#ifdef VERSION_ONE
		else if ( nsStrcmp ( webNames[xa], "autoparse" ) == 0 )
		{
			ximport.xautoparse[0] = 'Y';
		}

		/*--------------------------------------------------------------
			custom parse
		--------------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "parsefunc" ) == 0 )
		{
			snprintf ( ximport.xparsefunc, sizeof(ximport.xparsefunc), "%s", webValues[xa] );
		}

		/*--------------------------------------------------------------
			auto parse
		--------------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "headrows" ) == 0 )
		{
			ximport.xheadrows = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "colcount" ) == 0 )
		{
			ximport.xcolcount = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "refnum" ) == 0 )
		{
			ximport.xrefnum = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "date" ) == 0 )
		{
			ximport.xdate = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "payee" ) == 0 )
		{
			ximport.xpayee = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "increase" ) == 0 )
		{
			ximport.xincrease = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "incsign" ) == 0 )
		{
			ximport.xincsign[0] = 'Y';
		}
		else if ( nsStrcmp ( webNames[xa], "decrease" ) == 0 )
		{
			ximport.xdecrease = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "decsign" ) == 0 )
		{
			ximport.xdecsign[0] = 'Y';
		}
#endif

		/*--------------------------------------------------------------
			buttons
		--------------------------------------------------------------*/
		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "save_edit" ) == 0 )
			{
				RunMode = MODE_SAVE_EDIT;
			}
			else if ( nsStrcmp ( webValues[xa], "save_add" ) == 0 )
			{
				RunMode = MODE_SAVE_ADD;
			}
			else if ( nsStrcmp ( webValues[xa], "add_import" ) == 0 )
			{
				RunMode = MODE_ADD_IMPORT;
			}
			else if ( nsStrcmp ( webValues[xa], "delete_account" ) == 0 )
			{
				RunMode = MODE_DELETE_ACCOUNT;
			}
			else if ( nsStrcmp ( webValues[xa], "delete_import" ) == 0 )
			{
				RunMode = MODE_DELETE_IMPORT;
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

	switch ( AC_Mode )
	{
		case 'A':
		case 'C':
			break;
		case '?':
			Fatal ( "missing ac" );
			break;
		default :
			Fatal ( "invalid ac_mode" );
			break;
	}
}
