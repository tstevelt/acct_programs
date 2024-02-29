

delete from trx where trxnum >= 27 ;

update sys set trxnext = 27 ;

update imports set previous = '0000-00-00' ;

\! acct_chk
