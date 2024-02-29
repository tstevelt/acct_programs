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

#include	"rpttrx.h"

int ChkInput ()
{
//	snprintf ( title, sizeof(title), "%s", webValues[xa] );
//	snprintf ( rptname, sizeof(rptname), "%s", webValues[xa] );

	switch ( CategoryType )
	{
		case 'I':
		case 'X':
		case 'A':
		case 'L':
			break;
		default :
			SaveError ( "invalid type, use A L I or X" );
			break;
	}

	if ( RunMode == MODE_FIND )
	{
		if ( nsStrlen ( xrpt.xrptname ) == 0 )
		{
			SaveError ( "please enter report name" );
		}

		return ( AcctErrorCount );
	}

	if ( xrpt.xstartdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( xrpt.xenddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

	if ( Count == 0 )
	{
		SaveError ( "no categories selected" );
	}

//	sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
//	SaveError ( StatementOne );

	return ( AcctErrorCount );
}
