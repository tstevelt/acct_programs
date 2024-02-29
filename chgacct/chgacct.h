
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
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"

#define		MODE_START			101
#define		MODE_CHECK			102
#define		MODE_SAVE			103
TYPE	int		RunMode;
TYPE	long	CurrentAcctnum;
TYPE	long	NewAcctnum;
TYPE	char	CurrentName[31];
TYPE	char	NewName[31];

TYPE	COOKIE_RECORD	*AcctCookie;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

void GetInput ( void );
void PaintScreen ( void );
void ChangeAcctnum ( void );
int main ( int argc, char *argv[] );

/* ChkInput.c */
int ChkInput ( void );
