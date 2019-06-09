select 'Tester', 'TesterConfig', 'PackageSize', 'Initial_LoadBoard', 'Optimizer_LoadBoard', 'N_Lots', 'SUM_QTY', 'ChangeOver', 'Hours', 'PKGTYPE',    
                                 'PIN', 'CAP_PER_DAY', 'WIP_POST_WB', 'WIP_FT1', 'WIP_FT2', 'POST_WB_QUEUE',     
         'TEST_QUEUE_FT1', 'TEST_QUEUE_FT2', 'TOTAL_WIP'     
 UNION  
 select sp_hr_i.MI, sp_hr_i.MF, sp_hr_i.PKG_SIZE, coalesce( sp_hr_i.LB,'') as LB,    
                                 coalesce( case when (trim(sp_hr_i.TF) <> '') then sp_hr_i.TF else sp_hr_i.initial_tool_family end,'') as TF, 
                                 sp_hr_i.NROWS, sp_hr_i.SUM, sp_hr_i.RNUM, sp_hr_i.hours,                                          
                                 sp_hr_i.pkgtype, sp_hr_i.pin, sp_hr_i.cap_per_day, sp_hr_i.wip_post_WB, sp_hr_i.wip_ft1, sp_hr_i.wip_ft2,     
                                 sp_hr_i.post_wb_queue, sp_hr_i.test_queue_ft1, sp_hr_i.test_queue_ft2, sp_hr_i.total_wip  
 from (select sp_hr_a.*, i.* 
 from (select sp_hr.MI, sp_hr.MF, sp_hr.PKG_SIZE, sp_hr.LB as LB,    
                                 sp_hr.TF, sp_hr.NROWS, sp_hr.SUM, sp_hr.RNUM, sp_hr.hours,                                          
                                 so.pkgtype, so.pin, so.cap_per_day, so.wip_post_WB, so.wip_ft1, so.wip_ft2,     
                                 so.post_wb_queue, so.test_queue_ft1, so.test_queue_ft2, so.total_wip     
 from (select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, round(hr.hours,2) as hours, pin    
                 from (select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,     
                         if(coalesce(LB,'')<>TF, @intial := 1, @intial := 0) as initial,     
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
 		from ( select s.id, s.Machine_Instance, s.Machine_Family_Name, s.Package, case when s.Tooling_Family_Name='' then ini.initial_tool_family else s.Tooling_Family_Name  end as Tooling_Family_Name , s.Quantity,s.device_name, pin    
         from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name 
         left join initialsetup ini on ini.Machine_Instance=wip.MACHINE_INSTANCE) s    
 		left join     
 		(select machine_instance, initial_tool_family as atss_loadboard, machine_family, initial_certification as certification
		from optimizer.initialsetup
			group by 1,2
         ) dw on s.machine_instance =  dw.machine_instance   
 		group by s.Machine_Instance, s.Package,s.Tooling_Family_Name    
 		order by s.Machine_Instance, s.id    
 		) as a, (select @prev_MI := null,@prev_pkg_size := null,@prev_tf := null) i    
 		join (select @rownum := 0,@initail :=0) rw    
 		) sp    
 		left join    
 		(select hr.Machine_Instance,hr.package,hr.tooling_family_name,sum(hours) as hours from ( 
select hr.Machine_Instance,hr.package,case when hr.tooling_family_name='' then ini.Initial_Tool_Family else hr.tooling_family_name end as tooling_family_name , 
case when (trim(hr.Initial_Lot_Flag)='Y' and ini.hours is not null) then  
	ini.hours 
else 
sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date( 
	(case when substr(hr.Start_Time,1,2)='0/' then concat('12/',substr(hr.start_time,3,18)) else hr.start_time end),'%m/%d/%Y %H:%i:%s') ))/3600) end as hours 
from solution hr 
left join ( select wip.device,wip.cur_qty/r.pph as hours,i.machine_instance,i.initial_tool_family from wip  
left join initialsetup i on wip.Machine_Instance=i.Machine_Instance 
left join route r on r.Machine_Family=i.Machine_Family and r.tooling_family=i.initial_tool_family and wip.device=r.device 
where start_time<>'') ini on ini.device=hr.device_name and ini.machine_instance=hr.Machine_Instance 
group by hr.Machine_Instance, hr.package, hr.tooling_family_name 
) hr 
group by hr.Machine_Instance, hr.package, hr.tooling_family_name 
 		) hr on hr.Machine_Instance = sp.MI and hr.Package = sp.PKG_SIZE and hr.Tooling_Family_name = sp.TF   
 ) sp_hr 
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
  FROM (select s.id,s.Machine_Instance,s.Machine_Family_Name,s.Lot_Name,s.Logpoint,s.Quantity,s.Lot_Weight,s.Initial_Lot_Flag,s.Device_Name, 
            s.Certification,s.Setup_Time,s.Start_Time,s.Completion_Time,s.Pass_no,s.Finished,s.package, 
            case when s.Tooling_Family_Name='' then ini.initial_tool_family else s.Tooling_Family_Name  end as Tooling_Family_Name  
         from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name 
         left join initialsetup ini on ini.Machine_Instance=wip.MACHINE_INSTANCE 
         ) sol     
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
 ) so on so.machine_instance = sp_hr.MI and so.EDGE_LB = sp_hr.TF and so.pkg_size = sp_hr.PKG_SIZE 
 ) sp_hr_a 
left join (select i.machine_instance, i.initial_tool_family from initialsetup i order by i.machine_instance) i 
on i.machine_instance = sp_hr_a.MI 
) sp_hr_i 
;


"INTO OUTFILE 'C:/optimizer/output/summary_plan.csv' 
"FIELDS OPTIONALLY ENCLOSED BY ' ' TERMINATED BY ',' ESCAPED BY '' 
"LINES TERMINATED BY '\n' "