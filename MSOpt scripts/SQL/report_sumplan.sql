select * from wip;
select * from solution;

select MI, MF, PKG, LB, TF, NROWS, SUM, RNUM
from
(
select MI, MF, PKG, LB, TF, NROWS, SUM,
	case when MI = @prev_MI then (@rownum := @rownum + 1) else @rownum := 1 end as RNUM,
	@prev_MI := MI
from
(
select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,
	count(*) as NROWS, sum(s.Quantity) as SUM
from solution s
left join
(select * from smsdw) dw on s.device_name =  dw.device
group by s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name
order by s.Machine_Instance, s.Package
) as a, (select @prev_MI := null) i
cross join (select @rownum := 0) r
) z
;

(select 'MI', 'MF', 'PKG', 'LB', 'TF', 'NROWS', 'SUM', 'RNUM')
UNION
(select MI, MF, PKG, LB, TF, NROWS, SUM, RNUM
from
(
	select MI, MF, PKG, LB, TF, NROWS, SUM,
		case when MI = @prev_MI then (@rownum := @rownum + 1) else @rownum := 1 end as RNUM,
		@prev_MI := MI
	from
	(
		select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,
			count(*) as NROWS, sum(s.Quantity) as SUM
		from solution s
		left join
		(select * from smsdw) dw on s.device_name =  dw.device
		group by s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name
		order by s.Machine_Instance, s.Package
	) as a, (select @prev_MI := null) i
	cross join (select @rownum := 0) r
) z
INTO OUTFILE 'C:/temp/sum_plan.csv'  
FIELDS ENCLOSED BY '"' TERMINATED BY ',' ESCAPED BY ''  
LINES TERMINATED BY '\n')
;

commit;

select * from smsdw;


select atss_testec_config, alias, @prev_alias as prev_alias, 
	case when atss_testec_config = @prev_atss_testec_config then (@rownum := @rownum + 1) else @rownum := 1 end as row_number,
	@prev_alias := alias, @prev_atss_testec_config := atss_testec_config
from 
smsdw s, (select @prev_alias := null, @prev_atss_testec_config := null) i
cross join (select @rownum := 0) r
order by atss_testec_config
;

select * from solution where device_name in (select device from smsdw);