\! echo "customer list"
select * from arcust ;

select state, count(*) as count from arcust group by state order by count;

