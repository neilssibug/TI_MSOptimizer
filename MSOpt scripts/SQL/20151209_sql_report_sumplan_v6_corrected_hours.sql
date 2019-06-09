 /*
date modified: 2015-12-09

description:
corrected the hours computation based on Start_time and Completion_time in the solution.csv,
also for the grouping for it is now based on Machine_Instance, Package_Size and Tooling_Family

script:
select hr.Machine_Instance,hr.package,hr.tooling_family_name,
sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date(replace(trim(hr.start_time),'0/','12/'),'%m/%d/%Y %H:%i:%s') ))/3600) as hours
from solution hr
group by hr.Machine_Instance, hr.package, hr.tooling_family_name
 */
 
 select 'Tester', 'TesterConfig', 'PackageSize', 'Initial_LoadBoard', 'Optimizer_LoadBoard', 'N_Lots', 'SUM_QTY', 'ChangeOver', 'Hours', 'PKGTYPE',    
                                 'PIN', 'CAP_PER_DAY', 'WIP_POST_WB', 'WIP_FT1', 'WIP_FT2', 'POST_WB_QUEUE',     
         'TEST_QUEUE_FT1', 'TEST_QUEUE_FT2', 'TOTAL_WIP'    
 UNION 
 select sp_pph.MI, sp_pph.MF, sp_pph.PKG_SIZE, sp_pph.LB as LB,    
                                 sp_pph.TF, sp_pph.NROWS, sp_pph.SUM, sp_pph.RNUM, sp_pph.hours,                                         
                                 so.pkgtype, so.pin, so.cap_per_day, so.wip_post_WB, so.wip_ft1, so.wip_ft2,    
                                 so.post_wb_queue, so.test_queue_ft1, so.test_queue_ft2, so.total_wip    
 from (select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, round(hr.hours,2) as hours, pin    
                 from (select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,    
                         if(coalesce(LB,'')<>TF,@intial := 1,@intial := 0) as initial,    
 						if(MI = @prev_MI,    
 							if(@intial=1 and @rownum=0,    
 								@rownum :=1,     
                                 if(pkg_size <> @prev_pkg_size or TF<>@prev_TF,    
 									@rownum := @rownum + 1,    
                                     @rownum := @rownum    
                                 )    
                             )     
 						 , if(@intial = 1, @rownum := 1, @rownum :=0)    
                         )    
                         as rnum,@rownum as orig_data,    
 						pin,@prev_MI := MI,@prev_pkg_size := PKG_SIZE, @prev_tf := TF    
                 from (select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG_SIZE, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,    
 						count(*) as NROWS, sum(s.Quantity) as SUM, pin    
 		from (select s.id, s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name, s.Quantity,s.device_name, pin    
         from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name) s    
 		left join    
 		(select * from (select coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then 
				concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else t.tester_TWSTATUS_i end ),'') as machine_instance, s.atss_loadboard ,s.atss_testec_config as machine_family,     
 			case when s.atss_temp =25 then 1  when s.atss_temp >25 then 2 when s.atss_temp <25 then 3    end as certification     
 			from (select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (     
 			select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i,     
 			max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i      
             from twstatus t where substr(t.tester_TWSTATUS_i,1,4)='TT55'     
 			group by 1,2,t.last_update_TWSTATUS_i order by t.last_update_TWSTATUS_i desc) aa     
 			group by 2) t left join smsdw s  on t.lot_TWSTATUS_i = s.lot) aa where machine_family like 'ET%' group by 1,2    
         ) dw on s.machine_instance =  dw.machine_instance   
 		group by s.Machine_Instance, s.Package,s.Tooling_Family_Name    
 		order by s.Machine_Instance, s.id    
 		) as a, (select @prev_MI := null,@prev_pkg_size := null,@prev_tf := null) i    
 		join (select @rownum := 0,@initail :=0) rw    
 		) sp    
 		left join    
 		(select hr.Machine_Instance,hr.package,hr.tooling_family_name,
			sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date(
				(case when substr(hr.Start_Time,1,2)='0/' then concat('12/',substr(hr.start_time,3,18)) end),'%m/%d/%Y %H:%i:%s') ))/3600) as hours
			from solution hr
			group by hr.Machine_Instance, hr.package, hr.tooling_family_name
 		) hr on hr.Machine_Instance = sp.MI and hr.Package = sp.PKG_SIZE and hr.Tooling_Family_name = sp.TF    
 ) sp_pph    
 left join    
 (SELECT sol.machine_instance, pkg.pkgtype, sol.Machine_Family_Name as tester_req,     
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
    left join (select wip.device,wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip_cut wip      
   inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package)      
   group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.device=sol.device_name and pkg.lot_name = sol.Lot_Name       
  left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2))     
  left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name     
  left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name     
  left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name    
 group by sol.machine_instance, pkg.pkg_size,sol.Tooling_Family_Name    
 order by sol.Machine_Instance, sol.id    
 ) so on so.machine_instance = sp_pph.MI and so.EDGE_LB = sp_pph.TF and so.pkg_size = sp_pph.PKG_SIZE    
 ;
 INTO OUTFILE 'C:/optimizer/output/summary_plan.csv'      
 FIELDS OPTIONALLY ENCLOSED BY ' ' TERMINATED BY ',' ESCAPED BY ''      
 LINES TERMINATED BY '\n' 
 ;