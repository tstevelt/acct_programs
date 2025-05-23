PROG = entry
LIB = entry.a

#MYSQL_FLAGS = -lz -lcrypt -lnsl -lm -lssl -lcrypto
MYSQL_DIR = /usr/lib64
MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)
DEF = /usr/local/include
# PRG = /home/httpd/acct/html/$(PROG).cgi
PRG = /var/www/vhosts/acct/html/$(PROG).cgi

FILES = \
	$(LIB)(entry.o)			\
	$(LIB)(ChkInput.o)		\
	$(LIB)(DeleteTrx.o)		\
	$(LIB)(FindTrx.o)		\
	$(LIB)(Lookup.o)		\
	$(LIB)(GetInput.o)		\
	$(LIB)(LoadTrx.o)		\
	$(LIB)(PaintTrx.o)		\
	$(LIB)(PaintLookup.o)	\
	$(LIB)(SaveTrx.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)
	cp -pv $(PRG) /var/www/vhosts/shs_acct/html/

$(LIB): $(FILES)

$(FILES): entry.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/weblib.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)

