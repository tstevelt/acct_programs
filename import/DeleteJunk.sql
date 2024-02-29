--     Accounting Programs
-- 
--     Copyright (C)  2000-2024 Tom Stevelt
-- 
--     This program is free software: you can redistribute it and/or modify
--     it under the terms of the GNU Affero General Public License as
--     published by the Free Software Foundation, either version 3 of the
--     License, or (at your option) any later version.
-- 
--     This program is distributed in the hope that it will be useful,
--     but WITHOUT ANY WARRANTY; without even the implied warranty of
--     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--     GNU Affero General Public License for more details.
-- 
--     You should have received a copy of the GNU Affero General Public License
--     along with this program.  If not, see <https://www.gnu.org/licenses/>.


-- mysql -D acct -vv <  DeleteJunk.sql

delete from trxd where trxd.trxnum >= 99999999 ;

delete from trxh where trxh.trxnum >= 99999999 ;

update import set `previous` = (select max(trxdate) from trxh, trxd where trxh.trxnum = trxd.trxnum and trxd.acctnum = 1028) ;

update system set trxnext = (select 1+max(trxnum) from trxh) ;

