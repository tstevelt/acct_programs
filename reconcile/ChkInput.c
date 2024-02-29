/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"reconcile.h"

void ChkInput ()
{
	int		xe = 0;

	switch ( RunMode )
	{
		case MODE_START:
			break;

		case MODE_GET:
			if ( xaccount.xacctnum == 0 )
			{
				SaveError ( "please select an account." );
				xe++;
			}
			if ( StartDate.month == 0 )
			{
				SaveError ( "starting date is required." );
				xe++;
			}
			if ( EndDate.month == 0 )
			{
				SaveError ( "ending date is required." );
				xe++;
			}
			if ( xe > 0 )
			{
				RunMode = MODE_START;
			}
			break;

		case MODE_SAVE:
			break;
	}
}

#ifdef YAJJEJEJ

	for ( xa = 0; xa < webCount; xa++ )
	{
		webFixHex ( webValues[xa] );
		TrimRightAndLeft ( webValues[xa] );

		if ( nsStrcmp ( webNames[xa], "acctnum" ) == 0 )
		{
			xaccount.xacctnum = nsAtol ( webValues[xa] );
		}
		else if ( nsStrcmp ( webNames[xa], "StartDate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &StartDate );
		}
		else if ( nsStrcmp ( webNames[xa], "EndDate" ) == 0 )
		{
			StringToDateval ( webValues[xa], &EndDate );
		}
		else if ( nsStrcmp ( webNames[xa], "StartBalance" ) == 0 )
		{
			dblVal = nsAtof ( webValues[xa] );
			
			if ( dblVal >= 0.0 )
			{
				StartBalance = 100.0 * dblVal + 0.5;
			}
			else
			{
				StartBalance = 100.0 * dblVal - 0.5;
			}
		}
		else if ( nsStrcmp ( webNames[xa], "EndBalance" ) == 0 )
		{
			dblVal = nsAtof ( webValues[xa] );
			
			if ( dblVal >= 0.0 )
			{
				EndBalance = 100.0 * dblVal + 0.5;
			}
			else
			{
				EndBalance = 100.0 * dblVal - 0.5;
			}
		}

		/*--------------------------------------------------------------
			[rec_m_00002940] [on]
			[rec_m_00002941] [on]
		--------------------------------------------------------------*/
		else if ( nsStrncmp ( webNames[xa], "rec_", 4 ) == 0 )
		{
			if ( TrxCount >= MAXTRX )
			{
				SaveError ( "Exceeds MAXTRX" );
				return;
			}

			TrxArray[TrxCount].d.xtrxnum = nsAtol ( &webNames[xa][6] );
			TrxCount++;
		}

#ifdef FROM_ENTRY
		else if ( nsStrcmp ( webNames[xa], "edit" ) == 0 )
		{
			TrxArray[1].d.xtrxnum = nsAtoi ( webValues[xa] );
			OnQuit[0] = 'C';
		}
		else if ( nsStrcmp ( webNames[xa], "OnQuit" ) == 0 )
		{
			OnQuit[0] = toupper ( webValues[xa][0] );
		}

		else if ( nsStrcmp ( webNames[xa], "refnum_1" ) == 0 )
		{
			snprintf ( TrxArray[1].d.xrefnum, sizeof(TrxArray[1].d.xrefnum), "%s", webValues[xa] );
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
		else if ( nsStrncmp ( webNames[xa], "delete_", 7 ) == 0 )
		{
			TrxArray[TrxCount].c.delete = 1;
		}

		else if ( nsStrcmp ( webNames[xa], "lookup" ) == 0 )
		{
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
#endif

		else if ( nsStrcmp ( webNames[xa], "what" ) == 0 )
		{
			if ( nsStrcmp ( webValues[xa], "save" ) == 0 )
			{
			}
			else if ( nsStrcmp ( webValues[xa], "get" ) == 0 )
			{
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
#endif
