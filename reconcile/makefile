PROG = reconcile
LIB = reconcile.a

#MYSQL_FLAGS = -lz -lcrypt -lnsl -lm -lssl -lcrypto
MYSQL_DIR = /usr/lib64
MYSQL_FLAGS = -lz -lcrypt -lm -lssl -lcrypto
XLIB = /usr/local/lib64/acctlib.a /usr/local/lib64/weblib.a /usr/local/lib64/dbylib.a /usr/local/lib64/shslib.a $(MYSQL_DIR)/libmysqlclient.so $(MYSQL_FLAGS)
DEF = /usr/local/include
# WEBDIR=/home/httpd/acct/html
WEBDIR=/var/www/vhosts/acct/html
PRG = $(WEBDIR)/$(PROG).cgi

FILES = \
	$(LIB)(reconcile.o)		\
	$(LIB)(GetInput.o)		\
	$(LIB)(ChkInput.o)		\
	$(LIB)(LoadOpen.o)		\
	$(LIB)(PaintOpen.o)		\
	$(LIB)(PaintTop.o)		\
	$(LIB)(SaveEndDate.o)	\
	$(LIB)(Update.o)

.SILENT:

$(PRG): $(LIB) $(XLIB)
	echo "using gcc to load $(PRG)"
	gcc -o $(PRG) $(LIB) $(XLIB)
	cp -pv $(PRG) /var/www/vhosts/shs_acct/html/
	make js

$(LIB): $(FILES)

$(FILES): reconcile.h $(DEF)/acctlib.h $(DEF)/fileacct.h $(DEF)/weblib.h $(DEF)/dbylib.h $(DEF)/shslib.h

clean:
	rm -f $(LIB)

js:
	cp -v reconcile.js   $(WEBDIR)/reconcile.js
	cp -pv  reconcile.js /var/www/vhosts/shs_acct/html/

all:
	make clean
	make

.PRECIOUS: $(LIB)

