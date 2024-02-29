
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
#include	"rptlib.h"

#define		SYSTEM
#define		ACCOUNT
#define		IMPORT
#define		TRXH
#define		TRXD
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"

#define		MODE_START			101
#define		MODE_RUN			102

#define		MODE_QUIT			199
TYPE	int		RunMode;
TYPE	char	ReportFormat;

TYPE	char	CategoryType;

TYPE	DATEVAL	startdate;
TYPE	DATEVAL	enddate;
TYPE	char	rptname[21];
//TYPE	char	title[51];

TYPE	char	fnData[256];
TYPE	FILE	*fpData;
TYPE	char	fnBody[256];
TYPE	FILE	*fpBody;
TYPE	REPORT_OPTIONS		ReportOptions;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

int ChkInput ();
void GetInput ();
void PaintScreen ();
int main ( int argc, char *argv[] );
int getdata ();
void dorpt ();

