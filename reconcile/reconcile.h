
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
