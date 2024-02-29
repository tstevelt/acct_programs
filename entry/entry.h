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
#define		TRXH
#define		TRXD
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"

/*--------------------------------------------------------------
	sequence numbers run from 1 to 25.  they map directly
	into the array.  so array [0] is not used.
--------------------------------------------------------------*/

typedef struct
{
	char	iu_flag;
	char	xacctname[31];
	int		delete;
} CONTROL;

typedef struct
{
	XTRXD	d;
	CONTROL	c;
} TRXREC;

TYPE	TRXREC	TrxArray[MAXSEQNO+1];
TYPE	int		TrxCount;

#define		MODE_START			101
#define		MODE_PREV			102
#define		MODE_NEXT			103
#define		MODE_SAVE			104
#define		MODE_DELETE			105
#define		MODE_LOOKUP			106
#define		MODE_CLEAR			107
#define		MODE_FIND			108

#define		MODE_QUIT			199
TYPE	int		RunMode;
TYPE	char	TargetNumber[20];
TYPE	char	TargetName[20];
TYPE	char	OnQuit[2];
TYPE	int		HiddenCaptcha;
TYPE	int		DeleteCaptcha;

extern	XACCOUNT	AcctsArray[];
extern	int		AcctsCount;

/*----------------------------------------------------------
:.,$d
:r! mkproto -p *.c
----------------------------------------------------------*/

/* ChkInput.c */
void ChkInput ( void );

/* DeleteTrx.c */
int DeleteTrx ( long Trxnum );

/* entry.c */
int main ( int argc , char *argv []);

/* FindTrx.c */
long FindTrx ( long OnScreen , int direction );

/* GetInput.c */
void GetInput ( void );

/* LoadTrx.c */
int LoadTrx ( long Trxnum );

/* Lookup.c */
int Lookup ( void );

/* PaintLookup.c */
void PaintLookup ( void );

/* PaintTrx.c */
void PaintTrx ( void );

/* SaveTrx.c */
void SaveTrx ( void );
