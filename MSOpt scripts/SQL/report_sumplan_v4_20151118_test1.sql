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
group by sol.machine_instance, sol.Machine_Family_Name, sol.Tooling_Family_Name, pkg.pin, pkg.pkg_size
;