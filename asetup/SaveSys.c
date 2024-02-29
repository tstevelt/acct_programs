/*----------------------------------------------------------------------------
	Program : SaveSys.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Update system record
	Return  : 
----------------------------------------------------------------------------*/

#include	"asetup.h"

void SaveSys ()
{
	char	Fragment[80];

	sprintf ( StatementOne, 
"update system set name = '%s', address = '%s', city = '%s', state = '%s', \
zipcode = '%s', phone = '%s', twofactor = '%c', assetnext = %d, liabnext = %d, equitynext = %d, \
incomenext = %d, expensenext = %d, \
showamts = '%c', printdest = '%c', reconcile = '%c', \
closemethod = '%c', retainequity = %d, retainincome = %d",
	xsystem.xname, xsystem.xaddress, xsystem.xcity, xsystem.xstate,
	xsystem.xzipcode, xsystem.xphone,  xsystem.xtwofactor[0], 
	xsystem.xassetnext, xsystem.xliabnext, xsystem.xequitynext,
	xsystem.xincomenext, xsystem.xexpensenext,
	xsystem.xshowamts[0], xsystem.xprintdest[0], xsystem.xreconcile[0],
	xsystem.xclosemethod[0], xsystem.xretainequity, xsystem.xretainincome );

	if ( xsystem.xnumtrxnew >= 1 && xsystem.xnumtrxnew <= MAXSEQNO )
	{
		sprintf ( Fragment, ", numtrxnew = %d", xsystem.xnumtrxnew );
		strcat ( StatementOne, Fragment );
	}
	else
	{
		sprintf ( Fragment, "number new trx range 1 to %d", MAXSEQNO );
		SaveError ( Fragment );
	}

	if ( xsystem.xnumtrxedit >= 1 && xsystem.xnumtrxedit <= MAXSEQNO )
	{
		sprintf ( Fragment, ", numtrxedit = %d", xsystem.xnumtrxedit );
		strcat ( StatementOne, Fragment );
	}
	else
	{
		sprintf ( Fragment, "number edit trx range 1 to %d", MAXSEQNO );
		SaveError ( Fragment );
	}

	dbyUpdate ( "setup", &MySql, StatementOne, 0, LOGFILENAME );

	LoadSystem ();
}
