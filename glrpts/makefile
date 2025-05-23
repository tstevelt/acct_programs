PROG = glrpts
LIB = $(PROG).a
DEF = /usr/local/include

#MYSQL_FLAGS = -lz -lcrypt -lnsl -lm -lssl -lcrypto
# MYSQL_DIR = /usr/lib64
# MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto

# XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/rptlib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so /usr/local/lib64/libxlsxwriter.a $(MYSQL_FLAGS)

XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/dbylib.a /usr/local/lib64/rptlib.a /usr/local/lib64/shslib.a  \
		/usr/local/lib/libmysqlclient.a \
		/usr/local/lib/libcrypto.a \
		/usr/local/lib/libssl.a \
		/usr/local/lib/libxlsxwriter.a \
		/usr/local/Cellar/zlib/1.3.1/lib/libz.a

# libta_lib.a


DIR1 = /usr/local/bin
PRG1 = $(DIR1)/$(PROG)

FILES = \
	$(LIB)(glrpts.o)	\
	$(LIB)(dorpt_coa.o)	\
	$(LIB)(getdata_coa.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG1): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG1)"
	gcc -o $(PRG1) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): glrpts.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

