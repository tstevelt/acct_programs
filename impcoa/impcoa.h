
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

#define		LOGFILENAME		"/var/local/acct.log"


#define		MODE_START		101
#define		MODE_IMPORT		102
TYPE	int		RunMode;

#define		MAXTOKS		20
TYPE	char	*tokens[MAXTOKS];
TYPE	int		tokcnt;
TYPE	char	*token2[MAXTOKS];
TYPE	int		tokcn2;


/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/
void PaintScreen ( void );
int DoFile ( char *tempfn );
