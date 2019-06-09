SELECT device, target_amount, finished_amount, shortage, ((abs(shortage)/target_amount)*100) as percent_error  from keydevice_production 
where ((abs(shortage)/target_amount)*100) >= 10

group by device;

select * from solution;
select * from wip;
select * from route;
select * from machines;
select * from tooling;

select device, machine_family, tooling_family, Certification, logpoint, count(*) from route where device = '603V1RUNR-G2N#' group by 1,2,3,4,5;

select device, count(lot) from wip where device = '603V1RUNR-G2N#' group by 1;

select * from wip where device = '603V1RUNR-G2N#' group by 1;

select distinct tooling_family from route where device = '603V1RUNR-G2N#';

select * from tooling where tooling_family = '6528944B';

select * from solution where tooling_family_name = '6528944B';


select keyfil.device, keyfil.qty, sum(wip.cur_qty) from 
(
	select device, sum(quantity) as qty from 
		(
			SELECT device,((abs(shortage)/target_amount)*100) as percent_error  
            from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 
            group by device
		) keydev_prod
		inner join 
		(
			select * from solution
		) sol on keydev_prod.device = sol.device_name
		group by keydev_prod.device
) keyfil
inner join
(
	select * from wip
) wip on keyfil.device = wip.device and keyfil.device = '603V1RUNR-G2N#'
group by keyfil.device;


select keyfil.device, keyfil.qty keyprod_qty, wip.lot, wip.weight, sum(wip.cur_qty) wip_qty from 
(
	select device, sum(quantity) as qty from 
		(
			SELECT device,((abs(shortage)/target_amount)*100) as percent_error  
            from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 
            group by device
		) keydev_prod
		inner join 
		(
			select * from solution
		) sol on keydev_prod.device = sol.device_name
		group by keydev_prod.device
) keyfil
inner join
(
	select * from wip
) wip on keyfil.device = wip.device
group by keyfil.device
having keyprod_qty < wip_qty;





select device, sum(quantity) from 
(SELECT device,((abs(shortage)/target_amount)*100) as percent_error  from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 group by device) keydev_prod
inner join 
(select * from solution) sol on keydev_prod.device = sol.device_name
group by keydev_prod.device;


select * from 
(SELECT device,((abs(shortage)/target_amount)*100) as percent_error  from keydevice_production where ((abs(shortage)/target_amount)*100) > 10) aa
inner join 

 (SELECT pkg.pkgtype, sol.Machine_Family_Name as tester_req, 
 sol.Tooling_Family_Name as EDGE_LB 
 ,pkg.pkg_size, sol.Device_Name , pkg.pin as package 
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
 left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip 
 inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) 
 group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name  
 left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name 
 left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name 
 group by sol.package, sol.Tooling_Family_Name,sol.Machine_Family_Name,sol.Device_Name) 
  bb on bb.device_name <> aa.device