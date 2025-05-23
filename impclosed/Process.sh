#!/bin/sh

##
## USAGE: impclosed db asset shortterm longterm file
##

echo "Fidelity Margin"
impclosed acct 1030 5011 5012 Closed67.csv > LoadMargin.sql
mysql -D acct < LoadMargin.sql
acct_chk -d acct

echo "Fidelity IRA"
impclosed acct 1028 5009 5010 Closed39.csv > LoadIRA.sql
mysql -D acct < LoadIRA.sql
acct_chk -d acct

echo "Fidelity Roth"
impclosed acct 1032 5013 5014 Closed54.csv > LoadRoth.sql
mysql -D acct < LoadRoth.sql
acct_chk -d acct


# ls -l Load*.sql

# head -15 Load*.sql



