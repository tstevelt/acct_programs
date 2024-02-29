
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
#define		MODE_QUIT			199
TYPE	int		RunMode;

TYPE	COOKIE_RECORD	*AcctCookie;

TYPE	char		StatementOne[MAXSTATEMENT];
TYPE	char		StatementTwo[MAXSTATEMENT];

typedef struct
{
	long	InvoiceNumber;
	long	Total;
	long	Discount;
	long	Payment;
} STORE_RECORD;

#define		MAXLINES		100

TYPE	STORE_RECORD	StoreArray[MAXLINES];
TYPE	int				StoreCount;

TYPE	char	  CheckNumber[21];
TYPE	DATEVAL   CheckDate;
TYPE	double	  CheckAmount;

/*----------------------------------------------------------
:.,$d
:r! mkproto -p *.c
----------------------------------------------------------*/

/* ar_receipt.c */
int main ( int argc , char *argv []);

/* ChkInput.c */
int ChkInput ( void );

/* Find.c */
void Find ( void );

/* GetInput.c */
void GetInput ( void );

/* PaintScreen.c */
void PaintScreen ( void );

/* Save.c */
void Save ( void );
