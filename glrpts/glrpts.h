#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
/*
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<stdint.h>
#include	<libgen.h>		// for basename()
*/

#include   <mysql/mysql.h>
#include   <mysql/errmsg.h>

#include	"shslib.h"
#include	"dbylib.h"

#define		SYSTEM
#define		ACCOUNT
#define		IMPORT
#define		TRXH
#define		TRXD
#define		MEMBER
#include	"fileacct.h"
#include	"acctlib.h"


TYPE	int		RunMode;
#define		MODE_COA	1
#define		MODE_BS		2
#define		MODE_IS		3
#define		MODE_CF		4
#define		MODE_TAX	5
#define		MODE_TRX	6
#define		MODE_ALL	7

// TYPE	MYSQL	*MySql;

/*----------------------------
	prototypes
----------------------------*/
void getargs ( int argc, char *argv[] );
void dorpt_coa ();
int getdata_coa ();

