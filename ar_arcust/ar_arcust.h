//     Accounting Programs
// 
//     Copyright (C)  2000-2024 Tom Stevelt
// 
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU Affero General Public License as
//     published by the Free Software Foundation, either version 3 of the
//     License, or (at your option) any later version.
// 
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU Affero General Public License for more details.
// 
//     You should have received a copy of the GNU Affero General Public License
//     along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

#define	SAFETY_ON_DELETE

#define		SYSTEM
#define		ARCUST
#include	"fileacct.h"
#include	"acctlib.h"

#define		MODE_START			101
#define		MODE_LOAD			102
#define		MODE_LOOKUP			103
#define		MODE_INSERT			104
#define		MODE_FOUND			105
#define		MODE_UPDATE			106
#define		MODE_DELETE			107
#define		MODE_QUIT			199
TYPE	int		RunMode;

TYPE	COOKIE_RECORD	*AcctCookie;
#ifdef SAFETY_ON_DELETE
TYPE	int		HiddenSafety;
TYPE	int		AnswerSafety;
#endif

#ifdef MAIN
FIELD_RECORD	FieldArray [] = 
{
  { 0, 1, DATATYPE_LONG, (char *)&xarcust.xid, sizeof(xarcust.xid), "id", "ID Number" },
  { 1, 0, DATATYPE_STRING, xarcust.xname, sizeof(xarcust.xname), "name", "Name" },
  { 1, 0, DATATYPE_STRING, xarcust.xaddr1, sizeof(xarcust.xaddr1), "addr1", "Address 1" },
  { 0, 0, DATATYPE_STRING, xarcust.xaddr2, sizeof(xarcust.xaddr2), "addr2", "Address 2" },
  { 1, 0, DATATYPE_STRING, xarcust.xcity, sizeof(xarcust.xcity), "city", "City" },
  { 1, 0, DATATYPE_STRING, xarcust.xstate, sizeof(xarcust.xstate), "state", "State" },
  { 1, 0, DATATYPE_STRING, xarcust.xzipcode, sizeof(xarcust.xzipcode), "zipcode", "Zipcode" },
  { 1, 0, DATATYPE_STRING, xarcust.xphone, sizeof(xarcust.xphone), "phone", "Phone" },
  { 1, 0, DATATYPE_STRING, xarcust.xcontact, sizeof(xarcust.xcontact), "contact", "Contact" },
};
int		FieldCount = sizeof(FieldArray)/sizeof(FIELD_RECORD);
FK_RECORD		FkArray [] =
{
	{ "arinvh", "customer", "invoice" }
};
int		FkCount = sizeof(FkArray)/sizeof(FK_RECORD);
#else
extern	FIELD_RECORD	FieldArray [];
extern	int				FieldCount;
extern	FK_RECORD		FkArray [];
extern	int				FkCount;
#endif

/*----------------------------------------------------------
:.,$d
:r! mkproto -p *.c
----------------------------------------------------------*/

/* ar_arcust.c */
int main ( int argc , char *argv []);

/* ChkInput.c */
int ChkInput ( void );

/* Delete.c */
int Delete ( void );

/* Load.c */
int Load ( void );

/* Lookup.c */
int Lookup ( void );

/* GetInput.c */
void GetInput ( void );

/* Insert.c */
void Insert ( void );

/* PaintScreen.c */
void PaintScreen ( void );

/* Update.c */
void Update ( void );
