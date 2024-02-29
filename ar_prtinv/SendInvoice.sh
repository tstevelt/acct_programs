#!/bin/sh


if [ "$2" = '' -o ! -f /var/local/tmp/$1 ]
then
	echo "USAGE: SendInvoice.sh  datafile  InvNum"
	exit 1
fi

DATA=$1
INUM=$2

cp /var/local/tmp/$DATA invoice_$INUM.txt

txt2ps   invoice_$INUM.txt -of invoice_$INUM.ps -tm 0.0 -bm 0.0
ps2pdf invoice_$INUM.ps invoice_$INUM.pdf
SendAttached -to:tms@stevelt.com -a:invoice_$INUM.pdf -a:invoice_$INUM.txt

