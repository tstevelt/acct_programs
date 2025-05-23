/*----------------------------------------------------------------------------
	Program : Process.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Run the backup command
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

#include	"backup.h"

void Process ()
{
	char	*cp;
	char	CommandLine[1024];
	time_t	SystemTime;

	time ( &SystemTime );


#ifdef UNIX_VERSION
	if (( cp = getenv ( "DOCUMENT_ROOT" )) == NULL )
	{
		SaveError ( "Cannot find file store" );
		return;
	}

	sprintf ( CommandLine, "/usr/bin/mysqldump -h 127.0.0.1 %s > %s/%s/backup_%s_%ld.sql",
		SessionRecord.DbName, 
		cp, 
		SessionRecord.DbName, 
		SessionRecord.DbName,
		SystemTime );
#else
	char	*context;
	char	*DbName;
	if (( context = getenv ( "CONTEXT_DOCUMENT_ROOT" )) == NULL )
	{
		SaveError ( "Cannot find CONTEXT_DOCUMENT_ROOT" );
		return;
	}

	if (( cp = getenv ( "REQUEST_URI" )) == NULL )
	{
		SaveError ( "Cannot find REQUEST_URI" );
		return;
	}

	if ( strstr ( cp, "shs" ) != NULL )
	{
		DbName = "shs";
	}
	else if ( strstr ( cp, "acct" ) != NULL )
	{
		DbName = "acct";
	}
	else
	{
		SaveError ( "Unknown db in REQUEST_URI" );
		return;
	}

	sprintf ( CommandLine, "/usr/local/bin/mysqldump %s > %s/%s/%s/backup_%s_%ld.sql",
		DbName, 
		context,
		DbName, 
		DbName,
		DbName,
		SystemTime );
#endif

	// printf ( "&emsp; debug %s<br>", CommandLine );
	printf ( "&emsp;%s<br>", CommandLine );

	system ( CommandLine );
}
