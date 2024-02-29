#!/bin/sh
#     Accounting Programs
# 
#     Copyright (C)  2000-2024 Tom Stevelt
# 
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Affero General Public License as
#     published by the Free Software Foundation, either version 3 of the
#     License, or (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Affero General Public License for more details.
# 
#     You should have received a copy of the GNU Affero General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.


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

