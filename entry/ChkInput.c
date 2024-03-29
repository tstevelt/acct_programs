/*----------------------------------------------------------------------------
	Program : ChkInput.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Validate user input
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

#include	"entry.h"

void ChkInput ()
{
	int		xt;

	switch ( RunMode )
	{
#ifdef CHECK_IN_LOOKUP_FUNC
		case MODE_FIND:
//			if ( xtrxh.xtrxnum > 0 )
//			{
//				RunMode = MODE_LOAD;
//				break;
//			}

			for ( ndx = 0, xo = 0; ndx < FieldCount; ndx++ )
			{
				if ( lastchr ( FieldArray[ndx].buffer, FieldArray[ndx].length ) > 0 )
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
#endif

		case MODE_SAVE:
			if ( xtrxh.xtrxdate.month == 0 )
			{
				SaveError ( "missing or bad date." ); 
			}

			if ( xsystem.xclosedate.month > 0 )
			{
				if ( CompareDateval ( &xtrxh.xtrxdate, &xsystem.xclosedate ) <= 0 )
				{
					SaveError ( "date can not be before system closed date" );
				}
			}

			if ( nsStrlen ( xtrxh.xrefnum ) == 0 )
			{
				SaveError ( "missing reference." ); 
			}

			if ( nsStrlen ( TrxArray[1].d.xpayee ) == 0 )
			{
				SaveError ( "missing payee." ); 
			}

			for ( xt = 1; xt <= TrxCount; xt++ )
			{
				if ( xt > 1 && TrxArray[xt].d.xacctnum == 0 && TrxArray[xt].d.xamount == 0 )
				{
					continue;
				}

				if ( TrxArray[xt].d.xacctnum == 0 )
				{
					sprintf ( StatementOne, "missing %s.", xt == 1 ? "account" : "category" );
					SaveError ( StatementOne ); 
				}

				if ( TrxArray[xt].d.xamount == 0 )
				{
					SaveError ( "missing amount." ); 
				}
			}
			break;

		case MODE_DELETE:
			if ( HiddenCaptcha != DeleteCaptcha )
			{
				SaveError ( "Captcha codes do not match." ); 
			}

			break;
	}
}
