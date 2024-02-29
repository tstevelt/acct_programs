/*----------------------------------------------------------------------------
	Program : PaintScreen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint user input screen
	Return  : 
----------------------------------------------------------------------------*/


#include	"import.h"

typedef struct
{
	short	acctnum;
	char	acctname[30];
	DATEVAL	previous;
	char	website[257];
	char	login[21];
} RECORD;

#define		MAXACCOUNT	100
static	RECORD	MyArray[MAXACCOUNT];
static	int		MyCount;

void PaintScreen ( )
{
	DBY_QUERY		*Query;
	int				xa;

	/*--------------------------------------------------------------
		get accounts from database.  save in array, 'cuz we
		need 'em twice.
	--------------------------------------------------------------*/
	sprintf ( StatementOne, 
"select account.acctnum, account.acctname, import.previous, import.website, import.login from account, import where account.acctnum = import.acctnum and (accttype = 'A' or accttype = 'L') order by account.accttype, account.acctnum" );

	Query = dbySelect ( "import", &MySql, StatementOne, LOGFILENAME );

	MyCount = 0;
	while (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		if ( MyCount >= MAXACCOUNT )
		{
			sprintf ( StatementOne, "Exceeds MAXACCOUNT" );
			SaveError ( StatementOne );
			break;
		}

		MyArray[MyCount].acctnum = nsAtol ( Query->EachRow[0] );

		snprintf ( MyArray[MyCount].acctname,  sizeof(MyArray[MyCount].acctname), 
						"%s", Query->EachRow[1] );

		StringToDateval ( Query->EachRow[2], &MyArray[MyCount].previous );

		snprintf ( MyArray[MyCount].website,  sizeof(MyArray[MyCount].website), 
						"%s", Query->EachRow[3] );

		snprintf ( MyArray[MyCount].login,  sizeof(MyArray[MyCount].login), 
						"%s", Query->EachRow[4] );

		MyCount++;
	}

	/*--------------------------------------------------------------
		paint javascript
	--------------------------------------------------------------*/
	printf ( "<script language='JavaScript1.2'>\n" );

	printf ( "function SetFilename ( Obj )\n" );
	printf ( "{\n" );

	printf ( "var acctno = (Obj.value - 0);\n" );

	printf ( "for ( var xa = 0; xa < acctcount; xa++ )\n" );
	printf ( "{\n" );
	printf ( "if ( acctdetail[xa][0] == acctno )\n" );
	printf ( "{\n" );

/*--------------------------------------------------------------
	apparently not allowed to suggest filename to input type=file.

	Error: uncaught exception: [Exception... "Security error"  code: "1000" 
		nsresult: "0x805303e8 (NS_ERROR_DOM_SECURITY_ERR)" 
		location: "http://acct.iota.home/import.cgi Line: 18"]

xxx	printf ( "window.document.impform.import_filename.value = acctdetail[xa][1];\n" );
--------------------------------------------------------------*/

	/*--------------------------------------------------------------
		update the latest transaction date
	--------------------------------------------------------------*/
	printf ( "var ElementName = 'import_previous';\n" );
	printf ( "var TheElement = window.document.getElementById ( ElementName );\n" );
	printf ( "var MyNode = TheElement.childNodes[0];\n" );
	printf ( "MyNode.nodeValue = acctdetail[xa][1];\n" );

	/*--------------------------------------------------------------
		this updates the import_website_href
	--------------------------------------------------------------*/
	printf ( "ElementName = 'import_website_href';\n" );
	printf ( "TheElement = window.document.getElementById ( ElementName );\n" );
	printf ( "TheElement.href = acctdetail[xa][2];\n" );

	/*--------------------------------------------------------------
		this updates the screen text is between the a-tags
	--------------------------------------------------------------*/
	printf ( "ElementName = 'import_website_link_text';\n" );
	printf ( "TheElement = window.document.getElementById ( ElementName );\n" );
	printf ( "var MyNode = TheElement.childNodes[0];\n" );
	printf ( "MyNode = MyNode.childNodes[0];\n" );
	printf ( "MyNode.nodeValue = acctdetail[xa][2];\n" );

	/*--------------------------------------------------------------
		update the login name for the website
	--------------------------------------------------------------*/
	printf ( "ElementName = 'import_login';\n" );
	printf ( "TheElement = window.document.getElementById ( ElementName );\n" );
	printf ( "var MyNode = TheElement.childNodes[0];\n" );
	printf ( "MyNode.nodeValue = acctdetail[xa][3];\n" );

	printf ( "return;\n" );
	printf ( "}\n" );
	printf ( "}\n" );
	printf ( "}\n\n" );

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

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>account</td>\n" );
	printf ( "<td>\n" );

	printf ( "<select name='ImportAcctnum' onChange='javascript:SetFilename(this);'>\n" );

	printf ( "<option value='0'>--- choose account ---</option>\n" );

	for ( xa = 0; xa < MyCount; xa++ )
	{
		printf ( "<option value='%d'>%s</option>\n",
						MyArray[xa].acctnum,
						MyArray[xa].acctname );
	}


	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>latest transaction date</td>\n" );
	printf ( "<td id='import_previous'>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>website</td>\n" );

	printf ( "<td id='import_website_link_text'>" );
	printf ( "<a id='import_website_href' target='_blank'>&nbsp;</a>" );
	printf ( "</td>\n" );

	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>login</td>\n" );
	printf ( "<td id='import_login'>&nbsp;</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
		auto set transactions to either OPEN or BALANCE
		auto messes up for PNC bank.
		works good for TD Ameritrade and Fidelity
	----------------------------------------------------------*/
	// AutoStatus = 0;
	printf ( "<tr>\n" );
	printf ( "<td>set status automatically</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='AutoStatus'>\n" );
	printf ( "<option value='0'>New</option>\n" );
	printf ( "<option value='1'>Open</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
		control how much information appears in the results window
	----------------------------------------------------------*/
	// Verbose = 0;
	printf ( "<tr>\n" );
	printf ( "<td>verbose</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='Verbose'>\n" );
	printf ( "<option value='0'>Quiet</option>\n" );
	printf ( "<option value='1'>Verbose</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
		control how much information appears in the results window
	----------------------------------------------------------*/
	// ReportSkips = 1;
	printf ( "<tr>\n" );
	printf ( "<td>report skipped transactions</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='ReportSkips'>\n" );
	printf ( "<option value='1'>Yes</option>\n" );
	printf ( "<option value='0'>No</option>\n" );
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td>filename</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='file' name='import_filename' size='40'> &nbsp;\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*----------------------------------------------------------
		row
	----------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td colspan='2' align='center'>\n" );
	printf ( "<input type='button' name='submitImport' value='import' " );
	if ( xmember.xmrole[0] == ROLE_DEMO )
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

	/*--------------------------------------------------------------
		paint javascript array
	--------------------------------------------------------------*/
	// printf ( "<script language='JavaScript1.2'>\n" );
	webStartJava ();
	printf ( "var acctdetail = [ \n" );

	for ( xa = 0; xa < MyCount; xa++ )
	{
		printf ( "[ %d, '%d/%d/%d', '%s', '%s' ],\n",
			MyArray[xa].acctnum,
			MyArray[xa].previous.month,
			MyArray[xa].previous.day,
			MyArray[xa].previous.year2,
			MyArray[xa].website,
			MyArray[xa].login );
	}
	printf ( " ];\n" );

	printf ( "var acctcount = %d;\n", MyCount );
	// printf ( "</script>\n" );
	webEndJava ();

}
