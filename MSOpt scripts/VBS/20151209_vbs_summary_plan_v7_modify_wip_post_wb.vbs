'title: Summary Plan report
'date modified: 2015-12-09
'
'description:
'corrected the hours computation based on Start_time and Completion_time in the solution.csv,
'also for the grouping for it is now based on Machine_Instance, Package_Size and Tooling_Family
'
'script:
'select hr.Machine_Instance,hr.package,hr.tooling_family_name,
'sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date(replace(trim(hr.start_time),'0/','12/'),'%m/%d/%Y %H:%i:%s') ))/3600) as hours
'from solution hr
'group by hr.Machine_Instance, hr.package, hr.tooling_family_name
 
'################################################################################################################################

Const ForReading = 1
Const ForWriting = 2
'parameters
Dim local_folder :  local_folder = "c:\optimizer"

on error resume next
Dim folder 
Dim strCon
set mycn = CreateObject("ADODB.Connection")
set myrs = CreateObject("ADODB.Recordset")
connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=localhost;Database=optimizer;User=root; Password=root;"
mycn.Open connectionString

Set objFSO = CreateObject("Scripting.FileSystemObject")

'delete existing files.
objFSO.DeleteFile "c:\optimizer\output\summary_plan.csv"

WScript.StdOut.Write("Generating Summary Plan" & vbCrLf)

'generate summary_plan

sql="select 'Tester', 'TesterConfig', 'PackageSize', 'Initial_LoadBoard', 'Optimizer_LoadBoard', 'N_Lots', 'SUM_QTY', 'ChangeOver', 'Hours', 'PKGTYPE',    " & _
"							 'PIN', 'CAP_PER_DAY', 'WIP_POST_WB', 'WIP_FT1', 'WIP_FT2', 'POST_WB_QUEUE',     " & _
"	 'TEST_QUEUE_FT1', 'TEST_QUEUE_FT2', 'TOTAL_WIP'    " & _
"UNION " & _
"select sp_pph.MI, sp_pph.MF, sp_pph.PKG_SIZE, sp_pph.LB as LB,    " & _
"							 sp_pph.TF, sp_pph.NROWS, sp_pph.SUM, sp_pph.RNUM, sp_pph.hours,  " & _
"							 so.pkgtype, so.pin, so.cap_per_day, so.wip_post_WB, so.wip_ft1, so.wip_ft2,    " & _
"							 so.post_wb_queue, so.test_queue_ft1, so.test_queue_ft2, so.total_wip    " & _
"from (select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, round(hr.hours) as hours, pin    " & _
"			 from (select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,    " & _
"					 if(coalesce(LB,'')<>TF,@intial := 1,@intial := 0) as initial,    " & _
"					if(MI = @prev_MI,    " & _
"						if(@intial=1 and @rownum=0,    " & _
"							@rownum :=1,     " & _
"							 if(pkg_size <> @prev_pkg_size or TF<>@prev_TF,    " & _
"								@rownum := @rownum + 1,    " & _
"								 @rownum := @rownum    " & _
"							 )    " & _
"						 )     " & _
"					 , if(@intial = 1, @rownum := 1, @rownum :=0)    " & _
"					 )    " & _
"					 as rnum,@rownum as orig_data,    " & _
"					pin,@prev_MI := MI,@prev_pkg_size := PKG_SIZE, @prev_tf := TF    " & _
"			 from (select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG_SIZE, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,    " & _
"					count(*) as NROWS, sum(s.Quantity) as SUM, pin    " & _
"	from (select s.id, s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name, s.Quantity,s.device_name, pin    " & _
"	 from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name) s    " & _
"	left join    " & _
"	(select * from (select coalesce(trim(case when substr(t.tester_TWSTATUS_i,7,1)=0 then " & _
"			concat('CLFTEA',substr(t.tester_TWSTATUS_i,8,1)) else t.tester_TWSTATUS_i end ),'') as machine_instance, s.atss_loadboard ,s.atss_testec_config as machine_family,     " & _
"		case when s.atss_temp =25 then 1  when s.atss_temp >25 then 2 when s.atss_temp <25 then 3    end as certification     " & _
"		from (select max(last_update_TWSTATUS_i) as last_update_TWSTATUS_i,tester_TWSTATUS_i,lot_TWSTATUS_i from (     " & _
"		select concat('CLFTEA',substr(t.tester_TWSTATUS_i,5,2)) as tester_TWSTATUS_i ,t.lot_TWSTATUS_i,     " & _
"		max(t.last_update_TWSTATUS_i) as last_update_TWSTATUS_i      " & _
"		 from twstatus t where substr(t.tester_TWSTATUS_i,1,4)='TT55'     " & _
"		group by 1,2,last_update_TWSTATUS_i order by last_update_TWSTATUS_i desc) aa     " & _
"		group by 2) t left join smsdw s  on t.lot_TWSTATUS_i = s.lot) aa where machine_family like 'ET%' group by 1,2    " & _
"	 ) dw on trim(s.machine_instance) =  trim(dw.machine_instance)    " & _
"	group by s.Machine_Instance, s.Package,s.Tooling_Family_Name    " & _
"	order by s.Machine_Instance, s.id    " & _
"	) as a, (select @prev_MI := null,@prev_pkg_size := null,@prev_tf := null) i    " & _
"	join (select @rownum := 0,@initail :=0) rw    " & _
"	) sp    " & _
"	left join    " & _
"	(select hr.Machine_Instance,hr.package,hr.tooling_family_name,   " & _
"		sum(time_to_sec(timediff(str_to_date(replace(trim(hr.Completion_Time),'00/','12/'),'%m/%d/%Y %H:%i:%s' ) ,str_to_date(replace(trim(hr.start_time),'0/','12/'),'%m/%d/%Y %H:%i:%s') ))/3600) as hours  " & _
"		from solution hr  " & _
"		group by hr.Machine_Instance, hr.package, hr.tooling_family_name   " & _
"	) hr on hr.Machine_Instance = sp.MI and hr.Package = sp.PKG_SIZE and hr.Tooling_Family_name = sp.TF    " & _
") sp_pph    " & _
"left join    " & _
"(SELECT sol.machine_instance, pkg.pkgtype, sol.Machine_Family_Name as tester_req,     " & _
"sol.Tooling_Family_Name as EDGE_LB     " & _
",pkg.pkg_size , pkg.Pin, pkg.Pin as package   " & _    
", sum(sol.Quantity) as expected_output_2_days   " & _    
", sum(sol.Quantity)/2 as expected_avg_output_per_day   " & _    
", round(avg(cap.capacity),2) as cap_per_day      " & _
", COALESCE((wip_postwb.total_wip),0) as wip_post_WB   " & _  
", COALESCE((wip_ft1.total_wip),0) as wip_ft1        " & _
", COALESCE((wip_ft2.total_wip),0) as wip_ft2     " & _
", COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE     " & _
", COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1     " & _
", COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2     " & _
", round(coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0),2)  as total_wip     " & _
"FROM solution sol     " & _
"left join device_cap cap on cap.device=sol.Device_Name     " & _
"left join (select wip.device,wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip_cut wip      " & _
"inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package)      " & _
"group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.device=sol.device_name and pkg.lot_name = sol.Lot_Name       " & _
"left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2))     " & _
"left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name     " & _
"left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name     " & _
"left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name    " & _
"group by sol.machine_instance, pkg.pkg_size,sol.Tooling_Family_Name    " & _
"order by sol.Machine_Instance, sol.id    " & _
") so on trim(so.machine_instance) = trim(sp_pph.MI) and so.EDGE_LB = sp_pph.TF and so.pkg_size = sp_pph.PKG_SIZE     " & _
"INTO OUTFILE 'C:/optimizer/output/summary_plan.csv'      " & _
"FIELDS ENCLOSED BY '  ' TERMINATED BY ',' ESCAPED BY ''      " & _
"LINES TERMINATED BY '\n' "

myrs.open sql,mycn
WScript.StdOut.Write("Generating Summary Plan: Complete" & vbCrLf)

mycn.close
set mycn = nothing
set myrs = nothing