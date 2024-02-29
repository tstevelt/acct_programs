
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
#define		MEMBER
#define		ARCUST
#define		ARINVH
#define		ARINVL
#include	"fileacct.h"
#include	"acctlib.h"

#define		MODE_START			101
#define		MODE_FIND			102
#define		MODE_SAVE			103
#define		MODE_ADD_HEAD		104
#define		MODE_ADD_LINE		105
#define		MODE_QUIT			199
TYPE	int		RunMode;

TYPE	COOKIE_RECORD	*AcctCookie;

TYPE	char		StatementOne[MAXSTATEMENT];
TYPE	char		StatementTwo[MAXSTATEMENT];

typedef struct
{
	long	LineNumber;
	char	*Descr;
	long	Amount;
} STORE_RECORD;

#define		MAXLINES		100

TYPE	STORE_RECORD	StoreArray[MAXLINES];
TYPE	int				StoreCount;

/*----------------------------------------------------------
:.,$d
:r! mkproto -p *.c
----------------------------------------------------------*/

/* AddHead.c */
void AddHead ( void );

/* AddLine.c */
void AddLine ( void );

/* ar_entinv.c */
int main ( int argc , char *argv []);

/* ChkInput.c */
int ChkInput ( void );

/* Find.c */
int Find ( void );

/* GetInput.c */
void GetInput ( void );

/* PaintScreen.c */
void PaintScreen ( void );

/* Save.c */
void Save ( void );
