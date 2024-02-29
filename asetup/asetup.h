
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
#include	"acctlib.h"

#define		SYSTEM
#define		ACCOUNT
#define		MEMBER
#include	"fileacct.h"

#define		MODE_START			101
#define		MODE_SAVE			104
#define		MODE_QUIT			199
TYPE	int		RunMode;
TYPE	int		PageNumber;

TYPE	COOKIE_RECORD	*AcctCookie;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

void GetInput ( void );
void PaintSys ( void );
void SaveSys ( void );
int main ( int argc, char *argv[] );
int PaintUsers ( void );

/* ChkInput.c */
int ChkInput ( void );
