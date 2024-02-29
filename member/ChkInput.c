/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"member.h"

static int DebugChkInput = 0;

int ChkInput ()
{
	int		rv = 0;
	int		ndx, xo;
	XMEMBER	TestCustomer;
	DATEVAL	TestDate;
	
	switch ( RunMode )
	{
		case MODE_LOOKUP:
			if ( xmember.xid > 0 )
			{
				RunMode = MODE_LOAD;
				break;
			}
			for ( ndx = 0, xo = 0; ndx < FieldCount; ndx++ )
			{
				if ( nsStrcmp ( FieldArray[ndx].name, "Mrole" )  == 0 )
				{
					if ( xmember.xmrole[0] != '?' )
					{
						xo++;
					}
				}
				else if ( FieldArray[ndx].datatype == DATATYPE_MYSQL_DATE )
				{
					memcpy ( &TestDate, FieldArray[ndx].buffer, sizeof(DATEVAL) );
					if ( DebugChkInput )
					{
						printf ( "date %s %d/%d/%d<br>\n", FieldArray[ndx].name, TestDate.month, TestDate.day, TestDate.year2 );
					}
					if ( TestDate.month > 0 )
					{
						xo++;
					}
				}
				else if ( lastchr ( FieldArray[ndx].buffer, FieldArray[ndx].length ) > 0 )
				{
					xo++;
				}
			}
			if ( xo == 0 )
			{
				SaveError ( "Please enter something to search for." );
				SaveError ( "Wildcards (asterisk) are allowed." );
				RunMode = MODE_START;
				rv++;
			}
			break;

		case MODE_LOAD:
			if ( xmember.xid == 0 )
			{
				SaveError ( "Enter an ID to load" );
				RunMode = MODE_START;
				rv++;
			}
			break;

		case MODE_INSERT:
			if ( xmember.xid > 0 )
			{
				snprintf ( WhereClause, sizeof(WhereClause), "member.id = %ld", xmember.xid );
				if ( LoadMember ( WhereClause, &TestCustomer, 0 ) == 1 )
				{
					SaveError ( "Customer ID is already in use" );
					rv++;
					RunMode = MODE_START;
					break;
				}
			}
			/* fall-through */
		case MODE_UPDATE:
			for ( ndx = 0; ndx < FieldCount; ndx++ )
			{
				if ( FieldArray[ndx].keyfield )
				{
					continue;
				}
				if ( nsStrcmp ( FieldArray[ndx].name, "Mrole" )  == 0 )
				{
					if ( xmember.xmrole[0] == '?' )
					{
						sprintf ( msgbuf, "%s is required", FieldArray[ndx].label );
						SaveError ( msgbuf );
						rv++;
					}
				}
				else if ( FieldArray[ndx].required && lastchr ( FieldArray[ndx].buffer, FieldArray[ndx].length ) == 0 )
				{
					sprintf ( msgbuf, "%s is required", FieldArray[ndx].label );
					SaveError ( msgbuf );
					rv++;
				}
			}
			if ( rv > 0 && RunMode == MODE_INSERT )
			{
				RunMode = MODE_START;
			}
			else if ( rv > 0 && RunMode == MODE_UPDATE )
			{
				RunMode = MODE_FOUND;
			}
			break;

		case MODE_DELETE:
#ifdef SAFETY_ON_DELETE
			if ( HiddenSafety != AnswerSafety )
			{
				SaveError ( "Safety mis-match" );
				RunMode = MODE_FOUND;
				rv++;
				break;
			}
#endif
			for ( ndx = 0; ndx < FkCount; ndx++ )
			{
				snprintf ( WhereClause, sizeof(WhereClause), "%s = %ld", FkArray[ndx].child_field, xmember.xid );
				if ( dbySelectCount ( &MySql, FkArray[ndx].child_table, WhereClause, LOGFILENAME ) > 0 )
				{
					sprintf ( msgbuf, "Customer has %s records", FkArray[ndx].english );
					SaveError ( msgbuf );
					RunMode = MODE_FOUND;
					rv++;
				}
			}
			break;
	}

	return ( rv );
}
