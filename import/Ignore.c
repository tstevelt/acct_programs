/*----------------------------------------------------------------------------
	Program : Ignore.c
	Author  : Tom Stevelt
	Date    : 01/09/2025
	Synopsis: Ignore certain lines input file.
			  Initially hard coded for Fidelity.  
	Note!!! : Need to add field to import table,
			  becuase don't necessarily want to ignore the same words
			  for a different source (ie PNC vs Fidelity).
	Return  : 1 to ignore (skip), 0 to keep

	Who		Date		Modification
	---------------------------------------------------------------------

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

#include	"import.h"


char *strcasestr(const char *haystack, const char *needle);


typedef struct
{
	char	*Word;
} RECORD;

static	RECORD	Array [] =
{
	/*----------------------------
		Fidelity
	----------------------------*/
	{ "STOCK" },		
	{ "REINVEST" },
	{ "REDEMPTI" },
	{ "DISTRIBU" },

	/*----------------------------
		Schwab
	----------------------------*/
//	{ "BUY" }
};

static	int		Count = sizeof(Array)  / sizeof(RECORD);

int Ignore ( char *String )
{
	for ( int ndx = 0; ndx < Count; ndx++ )
	{
		if ( strcasestr ( String, Array[ndx].Word ) != NULL )
		{
			return ( 1 );
		}
	}

	return ( 0 );
}
