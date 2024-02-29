
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

#define		MODE_PAINT_CHANGE	101
#define		MODE_PAINT_FORGOT	102
#define		MODE_SAVE_CHANGE	103
#define		MODE_SAVE_FORGOT	104
#define		MODE_QUIT			199
TYPE	int		RunMode;

TYPE	COOKIE_RECORD	*AcctCookie;
TYPE	char		Database[256];
TYPE	char		MemberName[256];
TYPE	long		ExpireTime;

TYPE	char		*CurrentPassword;
TYPE	char		*NewPassword;
TYPE	char		*ConfirmPassword;


/*----------------------------------------------------------
:.,$d
:r! mkproto -p *.c
----------------------------------------------------------*/

void GetInput ( void );
void PaintScreen ( void );
int ProcessForgot ( void );
int ProcessChange ( void );
int main ( int argc, char *argv[] );
int ChkInput ( void );

