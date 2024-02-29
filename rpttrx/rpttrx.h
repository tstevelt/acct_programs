
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
#define		RPT
#define		RPTACCT
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"

#define		MODE_START			101
#define		MODE_RUN			102
#define		MODE_SAVE			103
#define		MODE_FIND			104

#define		MODE_QUIT			199
TYPE	int		RunMode;
TYPE	char	ReportFormat;

TYPE	char	CategoryType;

#define		MAXCATEGORY		200
TYPE	short	Array[MAXCATEGORY];
TYPE	short	Count;

TYPE	char	fnData[256];
TYPE	FILE	*fpData;
TYPE	char	fnBody[256];
TYPE	FILE	*fpBody;
TYPE	REPORT_OPTIONS		ReportOptions;

/*----------------------------------------------------------
:.,$d
:r! mkdoc -s *.c
----------------------------------------------------------*/

int SaveRpt ( char RptName[], char CategoryType );
int ChkInput ();
void GetInput ();
void PaintScreen ();
int main ( int argc, char *argv[] );
void Report ( FILE *fp );
int getdata ();
void dorpt ();
void LoadRptAccts ( char RptName[] );
