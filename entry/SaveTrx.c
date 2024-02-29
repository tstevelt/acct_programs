/*----------------------------------------------------------------------------
	Program : SaveTrx.c
	Author  : Tom Stevelt
	Date    : 2000-2024
	Synopsis: Insert new transaction or update existing one.
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

#include	"entry.h"

#define		DEBUGGER
#undef		DEBUGGER

void SaveTrx ()
{
	DBY_QUERY	*Query;
	int			xt;
	long		ThisAffected, TotalAffected;
	long		(*Function)();
	long		MaxTrx;
	short		DidInsert = 0;
	long		IncrementAmount;

	if ( xtrxh.xtrxnum == 0 )
	{
		LoadSystem ();
		xtrxh.xtrxnum = xsystem.xtrxnext;
		DidInsert = 1;
	}

#ifdef DEBUGGER
	printf ( "<pre>\n" );
	printf ( "trx %ld\n", xtrxh.xtrxnum );
#endif

	TotalAffected = 0;

	for ( xt = 1; xt <= TrxCount; xt++ )
	{
#ifdef DEBUGGER
		printf ( "%8ld %4d %-40.40s %10.2f\n",
			TrxArray[xt].d.xtrxnum,
			TrxArray[xt].d.xacctnum,
			TrxArray[xt].d.xpayee,
			(double) TrxArray[xt].d.xamount / 100.0 );
#endif
		if ( TrxArray[xt].d.xamount == 0 && TrxArray[xt].c.iu_flag == 'I' )
		{
			continue;
		}

		if ( TrxArray[xt].c.iu_flag == 'I' )
		{
			TrxArray[xt].d.xseqnum = xt;

			if ( TrxArray[xt].d.xseqnum == 1 )
			{
				xtrxh.xstatus = STATUS_PREENT;
				sprintf ( StatementOne,
					"insert into trxh (%s) values (%ld,%d,'%s','%04d-%02d-%02d')",
						FIELDS_TRXH,
						xtrxh.xtrxnum,
						xtrxh.xstatus,
						xtrxh.xrefnum,
						xtrxh.xtrxdate.year4,
						xtrxh.xtrxdate.month,
						xtrxh.xtrxdate.day );
							
				ThisAffected = dbyInsert ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
			}

			sprintf ( StatementOne,
				"insert into trxd (%s) values (%ld,%d,%ld,'%s',%ld)",
					FIELDS_TRXD,
					xtrxh.xtrxnum,
					TrxArray[xt].d.xseqnum,
					TrxArray[xt].d.xacctnum,
					TrxArray[xt].d.xpayee,
					TrxArray[xt].d.xamount );

			Function = dbyInsert;

			IncrementAmount = TrxArray[xt].d.xamount;
		}
		else
		{
			/*--------------------------------------------------------------
				load existing transaction before updating or deleting it.
			--------------------------------------------------------------*/
			if ( LoadOneTrxd ( xtrxh.xtrxnum, TrxArray[xt].d.xseqnum ) == 1 )
			{
				if ( TrxArray[xt].c.delete == 0 && TrxArray[xt].d.xacctnum == xtrxd.xacctnum )
				{
					/*--------------------------------------------------------------
						same category.  all we need to do is calc difference.
					--------------------------------------------------------------*/
					IncrementAmount = TrxArray[xt].d.xamount - xtrxd.xamount;
				}
				else
				{
					/*--------------------------------------------------------------
						delete or changing category.  IncrementAmount stores the 
						amount for the NEW category.
					--------------------------------------------------------------*/
					if ( TrxArray[xt].c.delete == 0 )
					{
						IncrementAmount = TrxArray[xt].d.xamount;
					}
					else
					{
						IncrementAmount = 0L;
					}

					/*--------------------------------------------------------------
						back out all of the old amount for the old category.
					--------------------------------------------------------------*/
					sprintf ( StatementOne,
						"update account set currbal = currbal - %ld where acctnum = %ld",
								xtrxd.xamount, xtrxd.xacctnum );

					ThisAffected = dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
					if ( ThisAffected == 0 )
					{
						sprintf ( StatementOne, "adjust currbal for %ld failed", xtrxd.xacctnum );
						SaveError ( StatementOne );
					}
				}
			}
			else
			{
				IncrementAmount = TrxArray[xt].d.xamount;
			}

			if ( TrxArray[xt].c.delete == 0 )
			{
				sprintf ( StatementOne,
					"update trxh set refnum = '%s', trxdate = '%04d-%02d-%02d' where trxnum = %ld",
						xtrxh.xrefnum,
						xtrxh.xtrxdate.year4,
						xtrxh.xtrxdate.month,
						xtrxh.xtrxdate.day,
						xtrxh.xtrxnum );

				ThisAffected = dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );

				sprintf ( StatementOne,
"update trxd set acctnum = %ld, payee = '%s', amount = %ld where trxnum = %ld and seqnum = %d",
						TrxArray[xt].d.xacctnum,
						TrxArray[xt].d.xpayee,
						TrxArray[xt].d.xamount,
						xtrxh.xtrxnum,
						TrxArray[xt].d.xseqnum );

				Function = dbyUpdate;
			}
			else
			{
				sprintf ( StatementOne,
					"delete from trxd where trxnum = %ld and seqnum = %d",
						xtrxh.xtrxnum, TrxArray[xt].d.xseqnum );

				Function = dbyDelete;
			}
		}

#ifdef DEBUGGER
		printf ( "%s\n", StatementOne );
#endif
		ThisAffected = Function ( "entry", &MySql, StatementOne, 0, LOGFILENAME );

		if ( ThisAffected == 1 && IncrementAmount != 0L )
		{
			TotalAffected += ThisAffected;

			sprintf ( StatementOne,
				"update account set currbal = currbal + %ld where acctnum = %ld",
							IncrementAmount, TrxArray[xt].d.xacctnum );

			ThisAffected = dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
			if ( ThisAffected == 0 )
			{
				sprintf ( StatementOne,
						"adjust currbal for %ld failed", TrxArray[xt].d.xacctnum );
				SaveError ( StatementOne );
			}

		}
#ifdef DEBUGGER
		else
		{
			printf ( "%ld %d, acct %ld not updated\n",
				TrxArray[xt].d.xtrxnum, TrxArray[xt].d.xseqnum, TrxArray[xt].d.xacctnum );
		}
#endif

	}

	if ( DidInsert == 1 && TotalAffected > 0 )
	{
		sprintf ( StatementOne, "select max(trxnum) from trxh" );

		Query = dbySelect ( "entry", &MySql, StatementOne, LOGFILENAME );

		if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
		{
			MaxTrx = nsAtol ( Query->EachRow[0] );

#ifdef DEBUGGER
			printf ( "max(trxnum) %ld\n", MaxTrx );
#endif
			MaxTrx ++;

			sprintf ( StatementOne, "update system set trxnext = %ld", MaxTrx );

			ThisAffected = dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
			if ( ThisAffected == 0 )
			{
				sprintf ( StatementTwo, "system.trxnext not updated, should be %ld", MaxTrx );
			}
		}
	}

	sprintf ( StatementOne,
		"select status from trxh where trxnum = %ld",
				xtrxh.xtrxnum );

#ifdef DEBUGGER
#define DEBUGGER
	printf ( "%s<br>\n", StatementOne );
#endif

	Query = dbySelect ( "entry", &MySql, StatementOne, LOGFILENAME );

	if (( Query->EachRow = mysql_fetch_row ( Query->Result )) != NULL )
	{
		xtrxh.xstatus = nsAtol ( Query->EachRow[0] );

#ifdef DEBUGGER
	printf ( "%d<br>\n", xtrxh.xstatus );
#endif
		if ( CalcTrxBalance ( xtrxh.xtrxnum ) == 0 )
		{
			if ( xtrxh.xstatus < STATUS_OPEN )
			{
				sprintf ( StatementOne, "update trxh set status = %d where trxnum = %ld", 
							STATUS_OPEN, xtrxh.xtrxnum );

				dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
			}
		}
		else
		{
			sprintf ( StatementOne, "update trxh set status = %d where trxnum = %ld", 
						STATUS_BALANCE, xtrxh.xtrxnum );

			dbyUpdate ( "entry", &MySql, StatementOne, 0, LOGFILENAME );
		}
	}


#ifdef DEBUGGER
	printf ( "</pre>\n" );
#endif
}
