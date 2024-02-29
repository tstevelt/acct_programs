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

#include	"chpw.h"

int ChkInput ()
{
	int		rv = 0;
	long	CurrentTime;

	switch ( RunMode )
	{
		case MODE_PAINT_FORGOT:
			time ( &CurrentTime );
			if ( ExpireTime < CurrentTime )
			{
				sprintf ( msgbuf, "Emailed link has expired. %ld < %ld", ExpireTime, CurrentTime );
				SaveError ( msgbuf );
				return ( -1 );
			}
			break;

		case MODE_SAVE_CHANGE:
			if ( nsStrlen ( CurrentPassword ) == 0 )
			{
				SaveError ( "Missing current password" );
				return ( -1 );
			}
			/* FALL-THROUGH */
		case MODE_SAVE_FORGOT:
			if (( nsStrlen ( NewPassword     ) == 0 ) ||
				( nsStrlen ( ConfirmPassword ) == 0 ))
			{
				SaveError ( "Password is required" );
				return ( -1 );
			}
			if ( nsStrcmp ( NewPassword, ConfirmPassword ) != 0 )
			{
				SaveError ( "Passwords do not match" );
				return ( -1 );
			}
			if (( rv = pw_policy ( NewPassword, 3 )) != 0 )
			{
				sprintf ( msgbuf, "%s", pw_policy_string ( rv ) );
				SaveError ( msgbuf );
				return ( -1 );
			}
			break;
	}
	
	return ( rv );
}
