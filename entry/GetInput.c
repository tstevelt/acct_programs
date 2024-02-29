/*----------------------------------------------------------------------------
	Program : GetInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Get user input from screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"entry.h"

void GetInput ()
{
	int		xa;
	double	dblVal;

	/*----------------------------------------------------------
		get user input
	----------------------------------------------------------*/
	webGetInput();

	RunMode = MODE_START;
	OnQuit[0]  = 'I';

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "trxnum" ) == 0 )
		{
			xtrxh.xtrxnum = TrxArray[1].d.xtrxnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "edit" ) == 0 )
		{
			xtrxh.xtrxnum = TrxArray[1].d.xtrxnum = nsAtoi ( webValues[xa] );
			OnQuit[0] = 'C';
		}
		else if ( nsStrcmp ( webNames[xa], "OnQuit" ) == 0 )
		{
			OnQuit[0] = toupper ( webValues[xa][0] );
		}

		else if ( nsStrcmp ( webNames[xa], "refnum_1" ) == 0 )
		{
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "trxdate_1" ) == 0 )
		{
			StringToDateval ( webValues[xa], &xtrxh.xtrxdate );
		}
		else if ( nsStrncmp ( webNames[xa], "acctnum_", 8 ) == 0 )
		{
			/*--------------------------------------------------------------
				-123456789-123456789
				acctnum_update_99
				acctnum_insert_99
			--------------------------------------------------------------*/
			TrxCount = nsAtoi ( &webNames[xa][15] );

			if ( TrxCount > 0 && TrxCount <= MAXSEQNO )
			{
				TrxArray[TrxCount].d.xacctnum = nsAtol ( webValues[xa] );
				TrxArray[TrxCount].d.xtrxnum  = TrxArray[0].d.xtrxnum;
				TrxArray[TrxCount].d.xseqnum  = TrxCount;
				if ( nsStrncmp ( &webNames[xa][7], "_insert_", 8 ) == 0 )
				{
					TrxArray[TrxCount].c.iu_flag = 'I';
				}
				else	
				{
					TrxArray[TrxCount].c.iu_flag = 'U';
				}
			}
		}
		else if ( nsStrcmp ( webNames[xa], "acctname_insert_2'" ) == 0 )
		{
			TrxArray[2].d.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrncmp ( webNames[xa], "acctname_", 9 ) == 0 )
		{
			/* nothing to do */
		}
		else if ( nsStrncmp ( webNames[xa], "payee_", 6 ) == 0 )
		{
			TrxCount = nsAtoi ( &webNames[xa][6] );

			if ( TrxCount > 0 && TrxCount <= MAXSEQNO )
			{
				snprintf ( TrxArray[TrxCount].d.xpayee, sizeof(TrxArray[TrxCount].d.xpayee), "%s",
									webValues[xa] );
			}
		}
		else if ( nsStrncmp ( webNames[xa], "amount_", 7 ) == 0 )
		{
			TrxCount = nsAtoi ( &webNames[xa][7] );

			if ( TrxCount > 0 && TrxCount <= MAXSEQNO )
			{
				dblVal = nsAtof ( webValues[xa] );
				
				if ( dblVal >= 0.0 )
				{
					TrxArray[TrxCount].d.xamount = 100.0 * dblVal + 0.5;
				}
				else
				{
					TrxArray[TrxCount].d.xamount = 100.0 * dblVal - 0.5;
				}
			}
		}
		else if ( nsStrncmp ( webNames[xa], "delete_", 7 ) == 0 )
		{
			TrxArray[TrxCount].c.delete = 1;
		}

		else if ( nsStrcmp ( webNames[xa], "lookup" ) == 0 )
		{
			RunMode = MODE_LOOKUP;
			snprintf ( xaccount.xacctname, sizeof(xaccount.xacctname), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "targetNumber" ) == 0 )
		{
			snprintf ( TargetNumber, sizeof(TargetNumber), "%s", webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "targetName" ) == 0 )
		{
			snprintf ( TargetName, sizeof(TargetName), "%s", webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "HiddenCaptcha" ) == 0 )
		{
			HiddenCaptcha = nsAtoi ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "DeleteCaptcha" ) == 0 )
		{
			DeleteCaptcha = nsAtoi ( webValues[xa] );
		}

		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "save" ) == 0 )
			{
				RunMode = MODE_SAVE;
			}
			else if ( nsStrcmp ( webValues[xa], "clear" ) == 0 )
			{
				RunMode = MODE_CLEAR;
			}
			else if ( nsStrcmp ( webValues[xa], "find" ) == 0 )
			{
				RunMode = MODE_FIND;
			}
			else if ( nsStrcmp ( webValues[xa], "next" ) == 0 )
			{
				RunMode = MODE_NEXT;
			}
			else if ( nsStrcmp ( webValues[xa], "prev" ) == 0 )
			{
				RunMode = MODE_PREV;
			}
			else if ( nsStrcmp ( webValues[xa], "delete" ) == 0 )
			{
				RunMode = MODE_DELETE;
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
