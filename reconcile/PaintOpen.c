/*----------------------------------------------------------------------------
	Program : PaintOpen.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Paint transactions list
	Return  : 
----------------------------------------------------------------------------*/

#include	"reconcile.h"

/*--------------------------------------------------------------
	create Check Box Name
--------------------------------------------------------------*/
static char *CBN ( int Index, char PlusMinusFlag )
{
	static	char 	Name[20];

	sprintf ( Name, "rec_%c_%08ld", PlusMinusFlag, TrxArray[Index].d.xtrxnum );

	return ( Name );
}

static void SectionHeading ( char AccountType, char PlusMinusFlag )
{
	printf ( "<tr>\n" );

	if ( PlusMinusFlag == 'p' )
	{
		if ( AccountType == 'A' )
		{
			printf ( "<th colspan='5' width='50%%'>deposits</th>\n" );
		}
		else if ( AccountType == 'L' )
		{
			printf ( "<th colspan='5' width='50%%'>charges</th>\n" );
		}
		else
		{
			printf ( "<th colspan='5' width='50%%'>plus</th>\n" );
		}
	}
	else 
	{
		if ( AccountType == 'A' )
		{
			printf ( "<th colspan='5' width='50%%'>checks/withdrawals</th>\n" );
		}
		else if ( AccountType == 'L' )
		{
			printf ( "<th colspan='5' width='50%%'>payments</th>\n" );
		}
		else
		{
			printf ( "<th colspan='5' width='50%%'>minus</th>\n" );
		}
	}

	printf ( "</tr>\n" );

	printf ( "<tr>\n" );
	printf ( "<th>ref</th>\n" );
	printf ( "<th>date</th>\n" );
	printf ( "<th>payee</th>\n" );
	printf ( "<th>amount</th>\n" );
	printf ( "<th>r</th>\n" );
	printf ( "</tr>\n" );

}

static void PaintTrx ( int xt, char PlusMinusFlag )
{
	char	*cbName;

	printf ( "<tr>\n" );

	printf ( "<td>%s</td>\n",  
				TrxArray[xt].h.xrefnum );

	printf ( "<td>%02d/%02d</td>\n",
				TrxArray[xt].h.xtrxdate.month, 
				TrxArray[xt].h.xtrxdate.day );

	printf ( "<td>%s</td>\n",
			TrxArray[xt].d.xpayee );

	printf ( "<td align='right'>%.2f</td>\n",
			(double ) TrxArray[xt].d.xamount / 100.0 );

	if ( TrxArray[xt].h.xstatus <= STATUS_OPEN )
	{
		cbName = CBN ( xt, PlusMinusFlag );

		printf ( "<td align='center'><input type='checkbox' name='%s' checked ", cbName );
/*
		printf ( " onChange='javascript:Adjust(%.2f,\"%s\",%s);'", 
				(double) TrxArray[xt].d.xamount / 100.0, 
				PlusMinusFlag == 'p' ? "PlusBalance" : "MinusBalance",
				cbName );
*/
		printf ( " onChange='javascript:Adjust(%d,%s);'", xt, cbName );

		printf ( "></td>\n" );
	}
	else
	{
		printf ( "<td align='center'>%s</td>\n", StatusDescr(TrxArray[xt].h.xstatus , 1) );
	}

	printf ( "</tr>\n" );
}

static void PaintButtonsAndBalance ( char PlusMinusFlag, long Balance )
{	
	printf ( "<tr>\n" );

	printf ( "<td colspan='3'>\n" );

	printf ( "<input type='button' value='check all' " );
	printf ( "onClick='javascript:SetMarks(\"rec_%c\");'>&nbsp;\n",
				PlusMinusFlag == 'p' ? 'p' : 'm' );

	printf ( "<input type='button' value='clear checks' " );
	printf ( "onClick='javascript:ClearMarks(\"rec_%c\");'>\n",
				PlusMinusFlag == 'p' ? 'p' : 'm' );

	printf ( "</td>\n" );

	printf ( "<td id='%s' align='right'>%.2f</td>\n",
			PlusMinusFlag == 'p' ? "PlusBalance" : "MinusBalance",
			(double) Balance / 100.0 );

	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );
}

int PaintOpen ( int CalledBy )
{
	int		xt, xc;
	long	MinusOpen, PlusOpen;
	long	MinusClosed, PlusClosed;
	long	MinusBalance, PlusBalance, TotalBalance;

	switch ( CalledBy )
	{
		case MODE_GET:
		case MODE_SAVE:
			break;
		default:
			printf ( "PaintOpen: bad value for CalledBy %d<br>\n", CalledBy );
			return ( 0 );
	}

#ifdef DEVELOPER
printf ( "<pre>\n" );
for ( xt = 0; xt < TrxCount; xt++ )
{
	printf ( "%-8.8s %02d/%02d %-40.40s %12.2f\n",
		TrxArray[xt].d.xrefnum,
		TrxArray[xt].d.xtrxdate.month, TrxArray[xt].d.xtrxdate.day,
		TrxArray[xt].d.xpayee,
		(double ) TrxArray[xt].d.xamount / 100.0 );
}
printf ( "</pre>\n" );
#endif

	MinusOpen = 0;
	PlusOpen  = 0;
	MinusClosed = 0;
	PlusClosed  = 0;
	MinusBalance = PlusBalance = TotalBalance = 0;

	printf ( "<table class='AppWide'>\n" );

	/*--------------------------------------------------------------
		plus trans
	--------------------------------------------------------------*/
	SectionHeading ( xaccount.xaccttype[0], 'p' );

	for ( xt = 0; xt < TrxCount; xt++ )
	{
		if ( TrxArray[xt].Sign == -1 )
		{
			continue;
		}
		if ( TrxArray[xt].h.xstatus <= STATUS_OPEN )
		{
			PlusOpen += TrxArray[xt].d.xamount;
		}
		else
		{
			PlusClosed += TrxArray[xt].d.xamount;
		}
		
		PaintTrx ( xt, 'p' );
	}

	/*--------------------------------------------------------------
		plus total
	--------------------------------------------------------------*/
	PlusBalance = PlusOpen + PlusClosed;
	PaintButtonsAndBalance ( 'p', PlusBalance );

	/*--------------------------------------------------------------
		minus trans
	--------------------------------------------------------------*/
	SectionHeading ( xaccount.xaccttype[0], 'm' );

	for ( xt = 0; xt < TrxCount; xt++ )
	{
		if ( TrxArray[xt].Sign == 1 )
		{
			continue;
		}
		if ( TrxArray[xt].h.xstatus <= STATUS_OPEN )
		{
			MinusOpen += TrxArray[xt].d.xamount;
		}
		else
		{
			MinusClosed += TrxArray[xt].d.xamount;
		}

		PaintTrx ( xt, 'm' );
	}


	/*--------------------------------------------------------------
		minus total
	--------------------------------------------------------------*/
	MinusBalance = MinusOpen + MinusClosed;
	PaintButtonsAndBalance ( 'm', MinusBalance );

	/*--------------------------------------------------------------
		overall balance
	--------------------------------------------------------------*/
	TotalBalance = StartBalance + PlusOpen + PlusClosed + MinusOpen + MinusClosed- EndBalance;

	printf ( "<td align='right' colspan='3'>balance</td>\n" );

	printf ( "<td id='TotalBalance' align='right'>%.2f</td>\n", 
			(double) TotalBalance / 100.0 );
	printf ( "<td>&nbsp;</td>\n" );
	printf ( "</tr>\n" );


	/*--------------------------------------------------------------
		save changes.
	--------------------------------------------------------------*/
	printf ( "<tr>\n" );
	printf ( "<td align='center' colspan='5'>" );
	printf ( "<input type='button' value='save' " );
	if ( xmember.xmrole[0] == ROLE_DEMO )
	{
		printf ( "disabled>\n" );
	}
	else
	{
		printf ( " onClick='javascript:what.value=\"save\";submit();'>" );
	}
	printf ( "</td>\n" );
	printf ( "</tr>\n" );

	/*--------------------------------------------------------------
		end of table
	--------------------------------------------------------------*/
	printf ( "</table>\n" );

	/*--------------------------------------------------------------
		javascript variables
	--------------------------------------------------------------*/
	webStartJava ();

	printf ( "var StartBalance = %.2f;\n", (double) StartBalance / 100.0 );
	printf ( "var EndBalance = %.2f;\n", (double) EndBalance / 100.0 );
	/*
	printf ( "var MinusOpen = %.2f;\n", (double) MinusOpen / 100.0 );
	printf ( "var PlusOpen = %.2f;\n", (double) PlusOpen / 100.0 );
	printf ( "var MinusClosed = %.2f;\n", (double) MinusClosed / 100.0 );
	printf ( "var PlusClosed = %.2f;\n", (double) PlusClosed / 100.0 );
	printf ( "var TotalBalance = %.2f;\n", (double) TotalBalance / 100.0 );
	*/
	printf ( "var TrxCount = %d;\n", TrxCount );
	xc = 1;
	printf ( "var TrxArray = [" );
	for ( xt = 0; xt < TrxCount; xt++ )
	{
		printf ( "[ %d, %.2f, %d ]%s",
			xt, 
			(double) TrxArray[xt].d.xamount / 100.0,
			TrxArray[xt].h.xstatus <= STATUS_OPEN ? 1 : 2,
			xt + 1 < TrxCount ? "," : "" );

		if ( xc < 6 )
		{
			xc++;
		}
		else
		{
			xc = 0;
			printf ( "\n" );
		}
	}
	printf ( "];\n" );

	webEndJava ();


//printf ( "cb %d mc %d pc %d mo %d po %d<br>\n",
//			CalledBy, MinusCount, PlusCount, MinusOpen, PlusOpen );



	/*--------------------------------------------------------------
		if saving, and all trx are checked, then return 1 to
		indicate that recondate should be updated.	
	--------------------------------------------------------------*/
	if (( CalledBy == MODE_SAVE ) &&
		( MinusOpen == 0        ) &&
		( PlusOpen  == 0        ))
	{
		return ( 1 );
	}

	return  ( 0 );

}
