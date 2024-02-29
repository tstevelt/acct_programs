PROG = ar_entinv
LIB = ar_entinv.a

MYSQL_DIR = /usr/lib64
MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)
DEF = /usr/local/include
PRG = /var/www/vhosts/acct/html/$(PROG).cgi

FILES = \
	$(LIB)(ar_entinv.o)		\
	$(LIB)(AddHead.o)		\
	$(LIB)(AddLine.o)		\
	$(LIB)(ChkInput.o)		\
	$(LIB)(Find.o)			\
	$(LIB)(GetInput.o)		\
	$(LIB)(PaintScreen.o)	\
	$(LIB)(Save.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)
	cp -pv $(PRG) /var/www/vhosts/shs_acct/html/

$(LIB): $(FILES)

$(FILES): ar_entinv.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/weblib.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

