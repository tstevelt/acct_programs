PROG = ar_receipt
LIB = ar_receipt.a
DEF = /usr/local/include

# MYSQL_DIR = /usr/lib64
# MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
# XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)
# PRG = /var/www/vhosts/acct/html/$(PROG).cgi

XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a  \
		/usr/local/lib/libmysqlclient.a \
		/usr/local/lib/libcrypto.a \
		/usr/local/lib/libssl.a \
		/usr/local/Cellar/zlib/1.3.1/lib/libz.a


#		/usr/local/lib/libxlsxwriter.a \
#		/usr/local/lib64/rptlib.a \
# libta_lib.a

DIR1 = /Users/tms/Sites/acct
DIR2 = /Users/tms/Sites/shs

PRG1 = $(DIR1)/$(PROG).cgi
PRG2 = $(DIR2)/$(PROG).cgi

FILES = \
	$(LIB)(ar_receipt.o)		\
	$(LIB)(ChkInput.o)		\
	$(LIB)(Find.o)			\
	$(LIB)(GetInput.o)		\
	$(LIB)(PaintScreen.o)	\
	$(LIB)(Save.o)

.SILENT:

$(PRG1): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG1)"
	gcc -o $(PRG1) $(LIB) $(XLIB)
	cp -pv $(PRG1) $(PRG2)

$(LIB): $(FILES)

$(FILES): ar_receipt.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/weblib.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

