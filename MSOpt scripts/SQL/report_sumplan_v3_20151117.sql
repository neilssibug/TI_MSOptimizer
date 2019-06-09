use optimizer;


(select 'MI', 'MF', 'PKG', 'LB', 'TF', 'NROWS', 'SUM', 'RNUM')
UNION
(
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
INTO OUTFILE 'C:/temp/summary_plan.csv'  
FIELDS ENCLOSED BY '"' TERMINATED BY ',' ESCAPED BY ''  
LINES TERMINATED BY '\n')
;


select sp_pph.MI, sp_pph.MF, sp_pph.PKG_SIZE, COALESCE((sp_pph.LB),so.EDGE_LB) as LB,
		sp_pph.TF, sp_pph.NROWS, sp_pph.SUM, sp_pph.RNUM, sp_pph.SUM_PPH,
        round((sp_pph.SUM/sp_pph.SUM_PPH),2) as APPROX,
        so.pkgtype, so.pin, so.cap_per_day, so.wip_post_WB, so.wip_ft1, so.wip_ft2,
        so.post_wb_queue, so.test_queue_ft1, so.test_queue_ft2, so.total_wip
from
(
	select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, r.SUM_PPH
	from
	(
	select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,
		case when MI = @prev_MI then (@rownum := @rownum + 1) else @rownum := 1 end as RNUM,
		@prev_MI := MI
	from
	(
		select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG_SIZE, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,
			count(*) as NROWS, sum(s.Quantity) as SUM
		from solution s
		left join
		(select * from smsdw) dw on s.device_name =  dw.device
		group by s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name
		order by s.Machine_Instance, s.Package
	) as a, (select @prev_MI := null) i
	cross join (select @rownum := 0) r
	) sp
    
	left join    
	
    (
	select r.package,r.Machine_Family,r.Tooling_Family,sum(r.pph) as SUM_PPH from 
	route r
	group by r.package,r.Machine_Family,r.Tooling_Family
	) r on r.Package = sp.PKG_SIZE and r.Machine_Family = sp.MF and r.Tooling_Family = sp.TF
) sp_pph

left join

(
SELECT sol.machine_instance, pkg.pkgtype, sol.Machine_Family_Name as tester_req, 
 sol.Tooling_Family_Name as EDGE_LB 
 ,pkg.pkg_size , pkg.Pin, pkg.Pin as package 
 , sum(sol.Quantity) as expected_output_2_days 
 , sum(sol.Quantity)/2 as expected_avg_output_per_day 
 , round(avg(cap.capacity),2) as cap_per_day  
 , COALESCE((wip_postwb.total_wip),0) as wip_post_WB 
 , COALESCE((wip_ft1.total_wip),0) as wip_ft1    
 , COALESCE((wip_ft2.total_wip),0) as wip_ft2 
 , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE 
 , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1 
 , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2 
 , coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0)  as total_wip 
 FROM solution sol 
 left join device_cap cap on cap.device=sol.Device_Name 
   left join (select wip.device,wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip  
  inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package)  
  group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.device=sol.device_name and pkg.lot_name = sol.Lot_Name   
 left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name
group by sol.machine_instance,pkg.pin,sol.Machine_Family_Name,pkg.pin,sol.Tooling_Family_Name
) so on so.machine_instance = sp_pph.MI and so.tester_req = sp_pph.MF and so.pkg_size = sp_pph.PKG_SIZE
;


select 'Tester', 'TesterConfig', 'PackageSize', 'Initial_LoadBoard', 'Optimizer_LoadBoard', 'N_Lots', 'SUM_QTY', 'ChangeOver', 'SUM_PPH', 'Hours', 'PKGTYPE', 
		'PIN', 'CAP_PER_DAY', 'WIP_POST_WB', 'WIP_FT1', 'WIP_FT2', 'POST_WB_QUEUE', 
        'TEST_QUEUE_FT1', 'TEST_QUEUE_FT2', 'TOTAL_WIP'
UNION

select sp_pph.MI, sp_pph.MF, sp_pph.PKG_SIZE, COALESCE((sp_pph.LB),so.EDGE_LB) as LB,
		sp_pph.TF, sp_pph.NROWS, sp_pph.SUM, sp_pph.RNUM, sp_pph.SUM_PPH,
		round((sp_pph.SUM/sp_pph.SUM_PPH),2) as APPROX,
		so.pkgtype, so.pin, so.cap_per_day, so.wip_post_WB, so.wip_ft1, so.wip_ft2,
		so.post_wb_queue, so.test_queue_ft1, so.test_queue_ft2, so.total_wip
from
(
	select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, r.SUM_PPH
	from
	(
	select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,
		case when MI = @prev_MI then (@rownum := @rownum + 1) else @rownum := 1 end as RNUM,
		@prev_MI := MI
	from
	(
		select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG_SIZE, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,
			count(*) as NROWS, sum(s.Quantity) as SUM
		from solution s
		left join
		(select * from smsdw) dw on s.device_name =  dw.device
		group by s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name
		order by s.Machine_Instance, s.Package
	) as a, (select @prev_MI := null) i
	 join (select @rownum := 0) r
	) sp
	
	left join    
	
	(
	select r.package,r.Machine_Family,r.Tooling_Family,sum(r.pph) as SUM_PPH from 
	route r
	group by r.package,r.Machine_Family,r.Tooling_Family
	) r on r.Package = sp.PKG_SIZE and r.Machine_Family = sp.MF and r.Tooling_Family = sp.TF
) sp_pph

left join

(
SELECT sol.machine_instance, pkg.pkgtype, sol.Machine_Family_Name as tester_req, 
 sol.Tooling_Family_Name as EDGE_LB 
 ,pkg.pkg_size , pkg.Pin, pkg.Pin as package 
 , sum(sol.Quantity) as expected_output_2_days 
 , sum(sol.Quantity)/2 as expected_avg_output_per_day 
 , round(avg(cap.capacity),2) as cap_per_day  
 , COALESCE((wip_postwb.total_wip),0) as wip_post_WB 
 , COALESCE((wip_ft1.total_wip),0) as wip_ft1    
 , COALESCE((wip_ft2.total_wip),0) as wip_ft2 
 , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE 
 , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1 
 , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2 
 , round(coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0),2)  as total_wip 
 FROM solution sol 
 left join device_cap cap on cap.device=sol.Device_Name 
   left join (select wip.device,wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip  
  inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package)  
  group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.device=sol.device_name and pkg.lot_name = sol.Lot_Name   
 left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name
group by sol.machine_instance,pkg.pin,sol.Machine_Family_Name,pkg.pin,sol.Tooling_Family_Name
) so on so.machine_instance = sp_pph.MI and so.EDGE_LB = sp_pph.TF and so.pkg_size = sp_pph.PKG_SIZE
INTO OUTFILE 'C:/optimizer/output/summary_plan.csv'  
FIELDS ENCLOSED BY '"' TERMINATED BY ',' ESCAPED BY ''  
LINES TERMINATED BY '\n'
;