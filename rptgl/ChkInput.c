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

#include	"rptgl.h"

int ChkInput ()
{
//	snprintf ( title, sizeof(title), "%s", webValues[xa] );
//	snprintf ( rptname, sizeof(rptname), "%s", webValues[xa] );

	switch ( CategoryType )
	{
		case 'B':
		case 'I':
			break;
		default :
			SaveError ( "invalid type, use 'B' or 'I'" );
			break;
	}

	if ( CategoryType == 'I' && startdate.month == 0 )
	{
		SaveError ( "missing starting date" );
	}

	if ( enddate.month == 0 )
	{
		SaveError ( "missing ending date" );
	}

//	sprintf ( StatementOne, "UNKNOWN [%s] [%s]", webNames[xa], webValues[xa] );
//	SaveError ( StatementOne );

	return ( AcctErrorCount );
}
