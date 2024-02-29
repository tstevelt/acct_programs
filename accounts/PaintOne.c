/*----------------------------------------------------------------------------
	Program : PaintOne.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint one account for editing
	Return  : 
----------------------------------------------------------------------------*/

#include	"accounts.h"

void PaintOne ()
{
	int		rv, xt;

	if ( RunMode == MODE_EDIT && LoadOneAccount ( xaccount.xacctnum ) == 0 )
	{
		sprintf ( StatementOne, "LoadOneAccount returned NULL." );
		SaveError ( StatementOne );
		return;
	}

	printf ( "<table class='AppWide'>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>type</td>\n" );
	printf ( "<td>\n" );
	if ( RunMode == MODE_ADD_ACCOUNT )
	{
		webStartJava ();
		printf ( "var NextNum = [ %d, %d, %d, %d, %d ];\n\n", 
			xsystem.xassetnext, xsystem.xliabnext, xsystem.xequitynext, 
			xsystem.xincomenext, xsystem.xexpensenext );

		printf ( "function SetAcctnum ( Obj )\n" );
		printf ( "{\n" );
		printf ( "var Type = Obj.value;\n" );
		printf ( "var Number = 0;\n" );
		printf ( "if ( Type == 'A' )\n" );
		printf ( "{\n" );
		printf ( "Number = NextNum [ 0 ];\n" );
		printf ( "}\n" );
		printf ( "else if ( Type == 'L' )\n" );
		printf ( "{\n" );
		printf ( "Number = NextNum [ 1 ];\n" );
		printf ( "}\n" );
		printf ( "else if ( Type == 'E' )\n" );
		printf ( "{\n" );
		printf ( "Number = NextNum [ 2 ];\n" );
		printf ( "}\n" );
		printf ( "else if ( Type == 'I' )\n" );
		printf ( "{\n" );
		printf ( "Number = NextNum [ 3 ];\n" );
		printf ( "}\n" );
		printf ( "else if ( Type == 'X' )\n" );
		printf ( "{\n" );
		printf ( "Number = NextNum [ 4 ];\n" );
		printf ( "}\n" );
		printf ( "window.document.accounts.account_acctnum.value = Number;\n" );
		printf ( "}\n" );

		webEndJava ();

		xaccount.xaccttype[0] = '?';

		printf ( "<select name='accttype' onChange='javascript:SetAcctnum(this);'>\n" );
		printf ( "<option value='?'>--- choose account type --- </option>\n" );

		if ( AC_Mode == 'A' )
		{
			printf ( "<option value='A'%s>asset </option>\n",
							xaccount.xaccttype[0] == 'A' ? " selected" : "" );
			printf ( "<option value='L'%s>liability </option>\n",
							xaccount.xaccttype[0] == 'L' ? " selected" : "" );
			printf ( "<option value='E'%s>equity </option>\n",
							xaccount.xaccttype[0] == 'E' ? " selected" : "" );
		}
		else
		{
			printf ( "<option value='I'%s>income </option>\n",
							xaccount.xaccttype[0] == 'I' ? " selected" : "" );
			printf ( "<option value='X'%s>expense </option>\n",
							xaccount.xaccttype[0] == 'X' ? " selected" : "" );
		}
		printf ( "</select>\n" );
	}
	else
	{
		switch ( xaccount.xaccttype[0] )
		{
			case 'A': printf ( "asset" ); break;
			case 'L': printf ( "liability" ); break;
			case 'E': printf ( "equity" ); break;
			case 'I': printf ( "income" ); break;
			case 'X': printf ( "expense" ); break;
		}
	}
	printf ( "</td>\n" );

	printf ( "<td>g/l number</td>\n" );
	if ( RunMode == MODE_ADD_ACCOUNT )
	{
		printf ( "<td> <input type='search' name='account_acctnum' size='10' value='%ld'> </td>\n", 
					xaccount.xacctnum );
	}
	else
	{
		printf ( "<td>%ld </td>\n", xaccount.xacctnum );
		printf ( "<input type='hidden' name='account_acctnum' value='%ld'>\n",
									xaccount.xacctnum );
	}
	printf ( "</tr>\n" );


	printf ( "<tr>\n" );
	printf ( "<td>name</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='acctname' size='30' value='%s'>\n",
					xaccount.xacctname );
	printf ( "</td>\n" );
	printf ( "<td>date range</td>\n" );
	printf ( "<td>\n" );
	printf ( "<select name='daterange'>\n" );
	for ( xt = TF_CUR_MONTH; xt <= TF_ALL; xt++ )
	{
		printf ( "<option value='%d'%s>%s</option>\n",
				xt,
				xt == xaccount.xdaterange ? " selected" : "",
				TimeFrameDescr ( xt ) );
	}
	printf ( "</select>\n" );
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>%s </td>\n",
					AC_Mode == 'A' ? "account number" : "tax form" );
	printf ( "<td> <input type='search' name='extranum' size='20' value='%s'> </td>\n",
					xaccount.xextranum );
	printf ( "<td>%s </td>\n",
					AC_Mode == 'A' ? "phone number" : "tax line" );
	printf ( "<td> <input type='search' name='extraline' size='20' value='%s'> </td>\n",
					xaccount.xextraline );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>opening balance</td>\n" );
	printf ( "<td> <input type='search' name='openbal' size='10' value='%.2f'> </td>\n",
					(double) xaccount.xopenbal / 100.0 );
	printf ( "<td>current balance</td>\n" );
	printf ( "<td> %.2f </td>\n", (double) xaccount.xcurrbal / 100.0 );
	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<td>last statement date</td>\n" );

	printf ( "<td>\n" );
	printf ( "<input type='search' name='recondate' size='9' maxlength='10'" );
	if ( xaccount.xrecondate.month > 0 )
	{
		printf ( " value='%02d/%02d/%02d'", 
				xaccount.xrecondate.month, xaccount.xrecondate.day, xaccount.xrecondate.year2 );
	}
	printf ( "></td>\n" );

	printf ( "<td>last statement balanace</td>\n" );
	printf ( "<td>\n" );
	printf ( "<input type='search' name='reconbal' size='12' maxlength='11'" );
//	if ( xaccount.xreconbal > 0 )
//	{
		printf ( " value='%.2f'", (double) xaccount.xreconbal / 100.0 );
//	}
	printf ( "></td>\n" );

	printf ( "</tr>\n" );

	if ( AC_Mode == 'C' )
	{
		rv = 1;
	}
	else if ( RunMode == MODE_EDIT )
	{
		rv = PaintImport ();
	}
	else
	{
		rv = 0;
	}

	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='4'>\n" );

	if ( RunMode == MODE_EDIT )
	{
		printf ( "<input type='button' value='delete account' " );
		if ( xmember.xmrole[0] == ROLE_DEMO )
		{
			printf ( "disabled>\n" );
		}
		else
		{
			printf ( "onClick='javascript:what.value=\"delete_account\";submit();'>\n" );
		}

		if ( rv != 0 )
		{
			printf ( "<input type='button' value='delete import' " );
			if ( xmember.xmrole[0] == ROLE_DEMO )
			{
				printf ( "disabled>\n" );
			}
			else
			{
				printf ( "onClick='javascript:what.value=\"delete_import\";submit();'>\n" );
			}
		}
	}

	printf ( "<input type='button' value='save' " );
	if ( xmember.xmrole[0] == ROLE_DEMO )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( "onClick='javascript:what.value=\"%s\";submit();'>\n",
						RunMode == MODE_EDIT ? "save_edit" : "save_add" );
	}

	if ( rv == 0 )
	{
		printf ( "&nbsp;<input type='button' value='add import' " );
		if (  xmember.xmrole[0] == ROLE_DEMO || RunMode == MODE_ADD_ACCOUNT )
		{
			printf ( " disabled>\n" );
		}
		else
		{
			printf ( "onClick='javascript:what.value=\"add_import\";submit();'>\n" ) ;
		}
	}

	printf ( "&nbsp;<input type='button' value='list' " );
	printf ( "onClick='javascript:window.open(\"accounts.cgi?ac=%c\",\"_self\");'>\n", AC_Mode );

	printf ( "</td>\n" );
	printf ( "</tr>\n" );
	printf ( "</table>\n" );

}
