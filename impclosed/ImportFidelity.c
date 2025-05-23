
#include	"impclosed.h"

void ImportFidelity ()
{
	FILE	*ifp;
	char	xbuffer[256];
	char	Statement[256];
	long	     ShortTerm,      LongTerm, ThisGain;
	long	TotalShortTerm, TotalLongTerm, TotalGain;
	int		xi, xl, xq, xm, lineno;

	if (( ifp = fopen ( Filename, "r" )) == (FILE *) 0 )
	{
		printf ( "can not open file [%s]\n", Filename );
		exit ( 1 );
	}

/*---------------------------------------------------------------------------
0             1                    2             3         4        5          6        7                    8
Symbol(CUSIP),Security Description,Date Acquired,Date Sold,Quantity,Cost Basis,Proceeds,Short Term Gain/Loss,Long Term Gain/Loss
undefined(064159SH0),BANK NOVA SCOTIA B CNOTE 2.00000% 11/15/2022,2022-04-19,2022-11-15,"35,000","$35,000.00","$35,000.00", --,$0.00,
undefined(3135G04Q3),FEDL NATL MTG ASSN SER BENCHMARK 0.25000% 05/22/2023,2022-11-02,2023-05-22,"35,000","$34,206.20","$35,000.00", --,$793.80,
undefined(3135G04Q3),FEDL NATL MTG ASSN SER BENCHMARK 0.25000% 05/22/2023,2022-04-14,2023-05-22,"10,000","$9,823.00","$10,000.00", --,$177.00,
undefined(3135G06H1),FEDL NATL MTG ASSN SER BENCHMARK 0.25000% 11/27/2023,2022-04-14,2023-11-27,"10,000","$9,747.00","$10,000.00", --,$253.00,
undefined(3135G0V75),FEDL NATL MTG ASSN SER BENCHMARK 1.75000% 07/02/2024,2022-04-14,2024-07-02,"10,000","$9,846.30","$10,000.00", --,$153.70,
undefined(459058CY7),INTERNATIONAL BANK FOR REC &DEV NOTE 2.12500% 02/13/2023 ISIN #US459058CY72 SEDOL #B9KBCS7,2022-04-19,2023-02-13,"35,000","$35,000.00","$35,000.00", --,$0.00,
undefined(540424AQ1),LOEWS CORP NOTE 2.62500% 05/15/2023,2022-04-19,2023-05-15,"35,000","$35,000.00","$35,000.00", --,$0.00,
undefined(857477AL7),STATE STR CORP NOTE 3.10000% 05/15/2023,2022-04-19,2023-05-15,"35,000","$35,000.00","$35,000.00", --,$0.00,
undefined(9128283C2),UNITED STATES TREAS SER AE-2022 2.00000% 10/31/2022 NTS NOTE,2022-04-13,2022-10-31,"9,000","$9,000.00","$9,000.00", --,$0.00,
---------------------------------------------------------------------------*/

	lineno = 0;
	TotalShortTerm = TotalLongTerm = TotalGain = 0;
	while ( fgets ( xbuffer, sizeof(xbuffer), ifp ) != NULL )
	{
		lineno++;
		if ( lineno == 1 )
		{
			continue;
		}

		if ( Verbose )
		{
			printf ( "\n-------------------------------------\n\n" );
			printf ( "-- %s", xbuffer );
		}
		/*---------------------------------------------------------------------------
			clean up quotes and internal commas
		undefined(064159SH0),BANK NOVA SCOTIA B CNOTE 2.00000% 11/15/2022,2022-04-19,2022-11-15,"35,000","$35,000.00","$35,000.00", --,$0.00,
		---------------------------------------------------------------------------*/
		xl = strlen ( xbuffer );
		xq = 0;

		for ( xi = 0; xi < xl; xi++ )
		{
			if ( xbuffer[xi] == '"' )
			{
				if ( xq == 0 )
				{
					xq = 1;
				}
				else
				{
					xq = 0;
				}
				for ( xm = xi; xm < xl; xm++ )
				{
					xbuffer[xm] = xbuffer[xm+1];
				}
				xl--;
			}
			else if ( xbuffer[xi] == ',' && xq == 1 )
			{
				for ( xm = xi; xm < xl; xm++ )
				{
					xbuffer[xm] = xbuffer[xm+1];
				}
				xl--;
			}
			else if ( xbuffer[xi] == '$' )
			{
				for ( xm = xi; xm < xl; xm++ )
				{
					xbuffer[xm] = xbuffer[xm+1];
				}
				xl--;
			}
		}

		if ( Verbose )
		{
			printf ( "-- %s", xbuffer );
		}

		if (( tokcnt = GetTokensD( xbuffer, ",\n\r", tokens, MAXTOKS )) != 9 )
		{
			fprintf ( stderr, "syntax line %d, tokens %d\n", lineno, tokcnt );
			continue;
		}	

		ShortTerm = (long)( 100.0 * atof ( tokens[7] ));
		LongTerm  = (long)( 100.0 * atof ( tokens[8] ));
		TotalShortTerm += ShortTerm;
		TotalLongTerm += LongTerm;
		ThisGain = ShortTerm + LongTerm;
		if ( ThisGain == 0 )
		{
			continue;
		}

#ifdef VERSION_1
		/*----------------------------------------------------------
			if bond or cd, extract cusip
			01234567890123456789
			undefined(064159SH0)
		----------------------------------------------------------*/
		if ( strncmp ( tokens[0], "undefined", 9 ) == 0 )
		{
			// sprintf ( xtrxh.xrefnum, "%9.9s", &tokens[0][10] );
			// sprintf ( xtrxh.xrefnum, "WTF" );
			// sprintf ( xtrxh.xrefnum, "%9.9s", tokens[0] + 10 );
			// cp = tokens[0];
			// cp += 10;
			// sprintf ( xtrxh.xrefnum, "%9.9s", cp );
			// GetTokensD ( tokens[0], "()", token2, MAXTOKS );
			// GetTokensD ( xbuffer, "()", token2, MAXTOKS );
			// sprintf ( xtrxh.xrefnum, "%9.9s", token2[1] );
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "%s", &tokens[0][10] );
		}
		else
		{
			for ( xi = 0; xi < strlen(tokens[0]); xi++ )
			{
				if ( tokens[0][xi] < 'A' )
				{
					break;
				}
				xtrxh.xrefnum[xi] = tokens[0][xi];
				xtrxh.xrefnum[xi+1] = '\0';
			}
		}
#endif
#ifdef VERSION_2
		if ( strncmp ( tokens[0], "undefined", 9 ) == 0 )
		{
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "BOND/CD" );
		}
		else if (( strncmp ( tokens[1], "CALL ", 5 ) == 0 ) ||
				 ( strncmp ( tokens[1], "PUT ", 4 ) == 0 ))
		{
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "OPTION" );
		}
		else if ( ThisGain < 0 )
		{
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "LOSS" );
		}
		else
		{
			snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "GAIN" );
		}

#endif

#define VERSION_3
#ifdef VERSION_3
		snprintf ( xtrxh.xrefnum, sizeof(xtrxh.xrefnum), "GAIN" );
#endif

		snprintf ( xtrxd.xpayee, sizeof(xtrxd.xpayee), "%s", tokens[1] );

		if ( Verbose )
		{
			printf ( "-- %s  %s  short %ld long %ld\n", xtrxh.xrefnum, xtrxd.xpayee, ShortTerm, LongTerm );
		}

		/*---------------------------------------------------------------------------
		+--------------+-------------+------+-----+------------+-------+
		| Field        | Type        | Null | Key | Default    | Extra |
		+--------------+-------------+------+-----+------------+-------+
		| name         | char(30)    | NO   |     |            |       |
		| address      | char(30)    | NO   |     |            |       |
		| city         | char(30)    | NO   |     |            |       |
		| state        | char(20)    | NO   |     |            |       |
		| zipcode      | char(20)    | NO   |     |            |       |
		| phone        | char(20)    | NO   |     |            |       |
		| assetnext    | smallint(6) | NO   |     | 1001       |       |
		| liabnext     | smallint(6) | NO   |     | 2001       |       |
		| equitynext   | smallint(6) | NO   |     | 3001       |       |
		| incomenext   | smallint(6) | NO   |     | 4001       |       |
		| expensenext  | smallint(6) | NO   |     | 6001       |       |
		| trxnext      | int(11)     | NO   |     | 1          |       |
		| showamts     | char(1)     | NO   |     | Y          |       |
		| closedate    | date        | NO   |     | 0000-00-00 |       |
		| reconcile    | char(1)     | NO   |     | N          |       |
		| printdest    | char(1)     | NO   |     | S          |       |
		| numtrxnew    | smallint(6) | YES  |     | 1          |       |
		| trxpermonth  | smallint(6) | NO   |     | 100        |       |
		| trxstartdate | date        | NO   |     | 0000-00-00 |       |
		| trxstarttrx  | int(11)     | NO   |     | 1          |       |
		| trxrollover  | smallint(6) | NO   |     | 0          |       |
		| closemethod  | char(1)     | NO   |     | M          |       |
		| retainequity | smallint(6) | YES  |     | NULL       |       |
		| retainincome | smallint(6) | YES  |     | NULL       |       |
		| numtrxedit   | smallint(6) | YES  |     | NULL       |       |
		| twofactor    | char(1)     | YES  |     | NULL       |       |
		+--------------+-------------+------+-----+------------+-------+
		+---------+-------------+------+-----+------------+-------+
		| Field   | Type        | Null | Key | Default    | Extra |
		+---------+-------------+------+-----+------------+-------+
		| trxnum  | int(11)     | NO   | PRI | 0          |       |
		| status  | smallint(6) | NO   |     | 1          |       |
		| refnum  | char(8)     | YES  |     | NULL       |       |
		| trxdate | date        | NO   |     | 0000-00-00 |       |
		+---------+-------------+------+-----+------------+-------+
		+---------+-------------+------+-----+---------+-------+
		| Field   | Type        | Null | Key | Default | Extra |
		+---------+-------------+------+-----+---------+-------+
		| trxnum  | int(11)     | NO   | PRI | 0       |       |
		| seqnum  | smallint(6) | NO   | PRI | 1       |       |
		| acctnum | int(11)     | NO   | MUL | 0       |       |
		| payee   | char(40)    | YES  |     | NULL    |       |
		| amount  | int(11)     | NO   |     | 0       |       |
		+---------+-------------+------+-----+---------+-------+
		---------------------------------------------------------------------------*/

		sprintf ( Statement,
			"insert into trxh ( trxnum, status, refnum, trxdate ) values ( %ld, %d, '%s', '%s' );",
				xsystem.xtrxnext,
				/* STATUS_ACCEPT */ STATUS_OPEN,
				xtrxh.xrefnum,
				tokens[3] );

		printf ( "%s\n", Statement );
		xtrxd.xseqnum = 1;
		
		sprintf ( Statement,
			"insert into trxd ( trxnum, seqnum, acctnum, payee, amount ) values ( %ld, %d, %ld, '%s', %ld );",
				xsystem.xtrxnext,
				xtrxd.xseqnum,
				AssetAccount, 
				xtrxd.xpayee,
				ThisGain );

		printf ( "%s\n", Statement );
		xtrxd.xseqnum++;

		if ( ShortTerm )
		{
			sprintf ( Statement,
				"insert into trxd ( trxnum, seqnum, acctnum, payee, amount ) values ( %ld, %d, %ld, '%s', %ld );",
					xsystem.xtrxnext,
					xtrxd.xseqnum,
					ShortTermAccount, 
					xtrxd.xpayee,
					ShortTerm );

			printf ( "%s\n", Statement );
			xtrxd.xseqnum++;
		}

		if ( LongTerm )
		{
			sprintf ( Statement,
				"insert into trxd ( trxnum, seqnum, acctnum, payee, amount ) values ( %ld, %d, %ld, '%s', %ld );",
					xsystem.xtrxnext,
					xtrxd.xseqnum,
					LongTermAccount, 
					xtrxd.xpayee,
					LongTerm );

			printf ( "%s\n", Statement );
			xtrxd.xseqnum++;
		}

		xsystem.xtrxnext++;
	}

	sprintf ( Statement, "update system set trxnext = %ld;", xsystem.xtrxnext );
	printf ( "%s\n", Statement );

	TotalGain = TotalShortTerm + TotalLongTerm;

	fprintf ( stderr, "Total short term gain %12.2f\n", (double)TotalShortTerm/100.0 );
	fprintf ( stderr, "Total long  term gain %12.2f\n", (double)TotalLongTerm/100.0 );
	fprintf ( stderr, "Total            gain %12.2f\n", (double)TotalGain/100.0 );

	fclose ( ifp );

}
