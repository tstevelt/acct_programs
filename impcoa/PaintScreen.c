/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/

#include	"impcoa.h"

void PaintScreen ( )
{
	/*--------------------------------------------------------------
		paint javascript
	--------------------------------------------------------------*/
	printf ( "<script language='JavaScript1.2'>\n" );

	printf ( "function DoImport ()\n" );
	printf ( "{\n" );
	printf ( "var options = 'screenX=10,screenY=10,width=600,height=500,resizable=1,menubar=1,scrollbars=1';\n" );
	printf ( "window.open('','impres',options);\n" );
	printf ( "window.document.impform.target = 'impres';\n" );
	printf ( "window.document.impform.submit();\n" );

	printf ( "}\n\n" );

	printf ( "</script>\n" );

	/*--------------------------------------------------------------
		paint html
	--------------------------------------------------------------*/

	printf ( "<table class='AppMedium'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center'>\n" );
	printf ( "<input type='radio' name='autonum' value='A'>\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "automatic numbering based on values entered in setup screen. import format is:<ol><li>account type, which is one of A, L, E, I or X.<li>account name<li>optional, tax form name<li>optional, tax form line</ol>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center'>\n" );
	printf ( "<input type='radio' name='autonum' value='M' checked>\n" );
	printf ( "</td>\n" );
	printf ( "<td>\n" );
	printf ( "account numbers from import file. import format is:<ol><li>account number.<li>account type, which is one of A, L, E, I or X.<li>account name<li>optional, tax form name<li>optional, tax form line</ol>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>filename</td>\n" );
	printf ( "<td colspan='2'>\n" );
	printf ( "<input type='file' name='import_filename' size='40'> &nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='4'>\n" );

	printf ( "<input type='button' name='submitImport' value='import' " );
	if ( xmember.xmrole[0] != ROLE_ADMIN )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:DoImport()'>\n" );
	}


	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "</table>\n" );


}
