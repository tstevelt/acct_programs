/*----------------------------------------------------------------------------
	Program : rpttrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Report of Assets, Liabilities, Income or Expense accounts

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
#include	"rpttrx.h"

int	UseErrorSys = 1;
static	int		DebugMain = 0;

int main ( int argc, char *argv[] )
{
	char			*ReportType;
	WEBPARMS	*ptrWebParms;
	char		WhereClause[128];

	AcctGetCookieAndSession ( &SessionRecord );

	GetInput ();

	switch ( RunMode  )
	{
		case MODE_START:
		case MODE_FIND:
			if ( CategoryType == 'A' )
			{
				StartUp ( "accounting - asset report", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}
			else if ( CategoryType == 'L' )
			{
				StartUp ( "accounting - liability report", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}
			else if ( CategoryType == 'I' )
			{
				StartUp ( "accounting - income report", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}
			else
			{
				StartUp ( "accounting - expense report", SessionRecord.DbName, 1, 1, 1, "javascript:Loaded();", GL_RPT_MENU );
			}

			ptrWebParms = webInitParms ();

			ptrWebParms->WP_FormName   = "rptform";
			ptrWebParms->WP_FormAction = "rpttrx.cgi";
			ptrWebParms->WP_FormMethod = "POST";
			ptrWebParms->WP_FormOther  = (char *)0;
			webStartForm ( ptrWebParms );

			webFreeParms ( ptrWebParms );

			printf ( "<input type=hidden name='type' value='%c'>\n", CategoryType );
			printf ( "<input type=hidden name='what' value='lunch'>\n" );

			break;

		case MODE_RUN:
			if ( CategoryType == 'A' )
			{
				ReportUp ( "asset report", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "asset" );
				ReportType = "Asset";
			}
			else if ( CategoryType == 'L' )
			{
				ReportUp ( "liability report", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "liability" );
				ReportType = "Liability";
			}
			else if ( CategoryType == 'I' )
			{
				ReportUp ( "income report", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "income" );
				ReportType = "Income";
			}
			else
			{
				ReportUp ( "expense report", SessionRecord.DbName, "javascript:Loaded();", ReportFormat, "expense" );
				ReportType = "Expense";
			}
			break;

		case MODE_SAVE:
			if ( CategoryType == 'A' )
			{
				StartUp ( "accounting - asset report", SessionRecord.DbName, 1, 1, 0, "", 0 );
			}
			else if ( CategoryType == 'L' )
			{
				StartUp ( "accounting - liability report", SessionRecord.DbName, 1, 1, 0, "", 0 );
			}
			else if ( CategoryType == 'I' )
			{
				StartUp ( "accounting - income report", SessionRecord.DbName, 1, 1, 0, "", 0 );
			}
			else
			{
				StartUp ( "accounting - expense report", SessionRecord.DbName, 1, 1, 0, "", 0 );
			}
			break;
	}

	switch ( RunMode )
	{
		case MODE_FIND:
			if ( ChkInput () == 0 )
			{
				if ( LoadOneRpt ( xrpt.xrptname, CategoryType ) == 0 )
				{
					SaveError ( "No report found with that name" );
				}
				else
				{
					Count = 0;
					LoadRptAccts ( xrpt.xrptname );
				}
			}
			/* fall through */

		case MODE_START:
			PaintScreen ();
			printf ( "</form>\n" );
			break;

		case MODE_RUN:
			if ( ChkInput () != 0 )
			{
				break;
			}

			if ( getdata () > 0 )
			{
				dorpt ( ReportType );

				switch ( ReportOptions.Format )
				{
					case RPT_FORMAT_CSV:
					case RPT_FORMAT_EXCEL:
					case RPT_FORMAT_TEXT:
					case RPT_FORMAT_PDF_EMAIL:
					case RPT_FORMAT_PDF_VIEW:
						sprintf ( fnBody, "%s/body_%d.txt", TEMPDIR, getpid() );
						if (( fpBody = fopen ( fnBody, "w" )) == (FILE *)0 )
						{
							printf ( "Cannot create %s for output\n", fnBody );
							break;
						}
						fprintf ( fpBody, "%s Report\n", ReportType );
						fprintf ( fpBody, "%s\n", xsystem.xname );
						fprintf ( fpBody, "%02d/%02d/%04d - %02d/%02d/%04d\n",
								xrpt.xstartdate.month, xrpt.xstartdate.day, xrpt.xstartdate.year4,
								xrpt.xenddate.month, xrpt.xenddate.day, xrpt.xenddate.year4 );
						nsFclose ( fpBody );

snprintf ( WhereClause, sizeof(WhereClause), "Mname = '%s'", SessionRecord.UserName );
LoadMember ( WhereClause, &xmember, 0 );
rptdeliv ( xmember.xmemail, xmember.xmemail, ReportType, fnBody, ReportOptions.OutputFilename, ReportOptions.Format, 0 );

						if ( DebugMain == 0 )
						{
							unlink ( fnBody );
						}
						break;
			 
					case RPT_FORMAT_HTML:
						rptcat ( ReportOptions.OutputFilename );
						
				}
			 
				if ( DebugMain == 0 )
				{
					// unlink ( fnData );
					// unlink ( ReportOptions.OutputFilename );
					printf ( "%s<br>\n", fnData );
					printf ( "%s<br>\n", ReportOptions.OutputFilename );
				}
													
			}
			break;

		case MODE_SAVE:
			/* rv = */ 
			SaveRpt ( xrpt.xrptname, CategoryType );
			/*
			switch ( rv )
			{
				case 0:
					printf ( "report unchanged<br>\n" );
					break;
				case 1:
					printf ( "report updated<br>\n" );
					break;
				case 2:
					printf ( "new report saved<br>\n" );
					break;
				default:
					printf ( "oops<br>\n" );
					break;
			}
			*/
			break;

		default :
			sprintf ( StatementOne, "RunMode %d not finished.", RunMode );
			SaveError ( StatementOne );
			break;
	}


	ErrorSysLoaded ( "Loaded" );

	printf ( "</body>\n" );
	printf ( "</html>\n" );
	
	dbyClose ( &MySql );

	return ( 0 );
}
