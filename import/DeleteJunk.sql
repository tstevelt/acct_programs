

-- mysql -D acct -vv <  DeleteJunk.sql

delete from trxd where trxd.trxnum >= 99999999 ;

delete from trxh where trxh.trxnum >= 99999999 ;

update import set `previous` = (select max(trxdate) from trxh, trxd where trxh.trxnum = trxd.trxnum and trxd.acctnum = 1028) ;

update system set trxnext = (select 1+max(trxnum) from trxh) ;

