/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
	Return  : 
----------------------------------------------------------------------------*/

#include	"ar_entinv.h"

int ChkInput ()
{
	int		rv = 0;
	
	switch ( RunMode )
	{
		case MODE_START:
		case MODE_QUIT:
			break;
		case MODE_FIND:
//			if ( xarinvh.xid == 0 )
//			{
//				SaveError ( "Enter invoice number to find" );
//				RunMode = MODE_START;
//			}
			break;
		case MODE_SAVE:
			break;
		case MODE_ADD_HEAD:
			if ( xarinvh.xcustomer == 0 )
			{
				SaveError ( "Enter customer number to add invoice" );
				RunMode = MODE_START;
				break;
			}
			snprintf ( WhereClause, sizeof(WhereClause), "arcust.id = %ld", xarinvh.xcustomer );
			if ( LoadArcust ( WhereClause, &xarcust, 0 ) != 1 )
			{
				SaveError ( "Unknown customer number" );
				xarinvh.xcustomer = 0;
				RunMode = MODE_START;
			}
			break;
		case MODE_ADD_LINE:
			break;
	}
	return ( rv );
}
