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

typedef struct
{
	XTRXH	h;
	XTRXD	d;
	int		Sign;
} TRXREC;

#define		MAXTRX	1024
TYPE	TRXREC	TrxArray[MAXTRX];
TYPE	int		TrxCount;

#define		MODE_START			101
#define		MODE_GET			102
#define		MODE_SAVE			103

#define		MODE_QUIT			199

TYPE	int		RunMode;
extern	XACCOUNT	AcctsArray[];
extern	int		AcctsCount;

TYPE	DATEVAL		StartDate;
TYPE	DATEVAL		EndDate;
TYPE	long		StartBalance;
TYPE	long		EndBalance;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

void LoadOpen ();
int PaintOpen ( int CalledBy );
void PaintTop ();
int main ( int argc, char *argv[] );
void Update ();
void GetInput ();
void ChkInput ();
void SaveEndDate ();
