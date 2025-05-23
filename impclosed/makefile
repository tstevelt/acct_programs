PROG = impclosed
LIB = impclosed.a
DEF = /usr/local/include
PRG = /usr/local/bin/$(PROG)

MYSQL_DIR = /usr/lib64
MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto

XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/dbylib.a  /usr/local/lib64/weblib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)

FILES = \
	$(LIB)(impclosed.o)	\
	$(LIB)(ImportFidelity.o)	\
	$(LIB)(getargs.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)

$(LIB): $(FILES)

$(FILES): impclosed.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

