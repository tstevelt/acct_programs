PROG = accounts
LIB = accounts.a

#MYSQL_FLAGS = -lz -lcrypt -lnsl -lm -lssl -lcrypto
# MYSQL_DIR = /usr/lib64
# MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
# XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)

XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a  \
		/usr/local/lib/libmysqlclient.a \
		/usr/local/lib/libcrypto.a \
		/usr/local/lib/libssl.a \
		/usr/local/Cellar/zlib/1.3.1/lib/libz.a


#		/usr/local/lib/libxlsxwriter.a \
#		/usr/local/lib64/rptlib.a \
# libta_lib.a


DEF = /usr/local/include
PRG = /Users/tms/Sites/acct/$(PROG).cgi

FILES = \
	$(LIB)(accounts.o)		\
	$(LIB)(AddAccount.o)	\
	$(LIB)(AddImport.o)		\
	$(LIB)(SaveAccount.o)	\
	$(LIB)(SaveImport.o)	\
	$(LIB)(GetInput.o)		\
	$(LIB)(PaintOne.o)		\
	$(LIB)(PaintImport.o)	\
	$(LIB)(PaintList.o)		\
	$(LIB)(Register.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)
	cp -pv $(PRG) /Users/tms/Sites/shs

$(LIB): $(FILES)

$(FILES): accounts.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/weblib.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

