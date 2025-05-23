/*----------------------------------------------------------------------------
	Program : impclosed.c
	Author  : Tom Stevelt
	Date    : 01/13/2025
	Synopsis: Import realized gains (taking profit or losses).
			  This is a command line program, not on the web app.
	Note	: Initially hard-coded for Fidelity only.

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


#define		MAIN
#include	"impclosed.h"

int main ( int argc, char *argv[] )
{
	int		rv;

	getargs ( argc, argv );

	if ( access ( Filename, R_OK ) != 0 )
	{
		printf ( "unknown file [%s]\n", Filename );
		exit ( 1 );
	}

	if (( rv = dbyInitNoExit ( &MySql, "localhost", "tms", "fleeing560vodka", Database, 3306, LOGFILENAME )) != 0 )
	{
		printf ( "unknown database %s, rv %d\n", Database, rv );
		exit ( 1 );
	}

	if ( LoadSystem () != 1 )
	{
		printf ( "cannot load system record\n" );
		exit ( 1 );
	}

	if ( LoadOneAccount ( AssetAccount ) != 1 )
	{
		printf ( "unknown asset account number %ld\n", AssetAccount );
		exit ( 1 );
	}

	if ( LoadOneAccount ( ShortTermAccount ) != 1 )
	{
		printf ( "unknown income account number %ld\n", ShortTermAccount );
		exit ( 1 );
	}

	ImportFidelity ();

	dbyClose ( &MySql );

	return ( 0 );
}
