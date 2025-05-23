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

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netinet/tcp.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<signal.h>
#include	<time.h>
#include	<errno.h>

#ifdef	APPLE
#include   <mysql.h>
#include   <errmsg.h>
#else
#include   <mysql/mysql.h>
#include   <mysql/errmsg.h>
#endif

#include	"shslib.h"
#include	"weblib.h"
#include	"dbylib.h"

#define		SYSTEM
#define		ACCOUNT
#define		IMPORT
#define		TRXD
#define		TRXH
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"


TYPE	XACCOUNT	MyAccount;
TYPE	XIMPORT		MyImport;
TYPE	XTRXH	NewTrxh;
TYPE	XTRXD	NewTrxd;
	
typedef struct
{
	char	*Name;
	int		(*Function)();
} PARSEFUNC;


typedef struct
{
	char		*Refnum;
	DATEVAL		Date;
	double		Amount;
	char		*Payee;
} RESULT;


#define		MODE_START		101
#define		MODE_IMPORT		102
TYPE	int		RunMode;

TYPE	short	ImportAcctnum;
TYPE	char	import_filename[128];
TYPE	int		Verbose;
TYPE	int		AutoStatus;
TYPE	int		ReportSkips;

#define		MAXTOKS		25
TYPE	char	*tokens[MAXTOKS];
TYPE	int		tokcnt;
TYPE	char	*token2[MAXTOKS];
TYPE	int		tokcn2;
TYPE	int		ReportCount, DateOkay, DateCount, DateClosed, DatePrevious;
TYPE	int		ImportCount, AcctErrorCount;
TYPE	long	FirstTrx, LastTrx;
TYPE	DATEVAL	MaxDate;

TYPE	char	OriginalRefnum[512];
TYPE	char	OriginalPayee[512];


/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

int main ( int argc, char *argv[] );
int DoFile ( char *tempfn );
int FindRepeating ( int Method, XTRXH *NewTrxh, XTRXD *NewTrxd, long *ThatTrxNumber );
int FindTrx ( long *trxnum, short *seqnum );
char *FixMoney ( char *Token );
void GetInput ();
void PaintScreen ( );
int AutoParse ( char *Buffer, RESULT *Result );
int ParseChaseCC ( char *Buffer, RESULT *Result );
int ParseNatCityCC ( char *Buffer, RESULT *Result );
int ParsePNC ( char *Buffer, RESULT *Result );

/* Ignore.c */
int Ignore ( char *String );

/* ImportCSV.c */
int ImportCSV ( char *Filename );

/* ImportOFX.c */
int ImportOFX ( char *Filename );

/* ImportQIF.c */
int ImportQIF ( char *Filename );

/* SaveTransaction.c */
void SaveTransaction ( void );
