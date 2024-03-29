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
#define		TRXH
#define		TRXD
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"

typedef struct
{
	char	*Name;
	int		(*Function)();
} PARSEFUNC;

#define		MODE_START				101
#define		MODE_EDIT				102
#define		MODE_ADD_ACCOUNT		103
#define		MODE_ADD_IMPORT			104
#define		MODE_SAVE_EDIT			105
#define		MODE_SAVE_ADD			106
#define		MODE_DELETE_ACCOUNT		107
#define		MODE_DELETE_IMPORT		108
#define		MODE_REGISTER			109

#define		MODE_QUIT				199
TYPE	int		RunMode;
TYPE	int		AC_Mode;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

int main ( int argc, char *argv[] );
int AddAccount ( void );
int AddImport ( short AccountNumber );
int SaveAccount ( void );
int SaveImport ( int AccountNumber );
void GetInput ( void );
void PaintList ( void );
void PaintOne ( void );
int PaintImport ( void );
void Register ( void );




