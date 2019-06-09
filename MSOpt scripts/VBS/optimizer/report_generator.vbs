'generate report
'MOD: MIke
'Date: 2015-09-08
'DESC: Generate report
'##################################################################################
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
objFSO.DeleteFile "c:\optimizer\output\optimizer_summary_report.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_summary_report_no_device.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_queue_by_loadboard.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_package_report.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_tester_config_report.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_loadboard_setup_change.csv"
objFSO.DeleteFile "c:\optimizer\output\optimizer_detailed_report.csv"
objFSO.DeleteFile "c:\optimizer\output\device_logpoint.csv"
objFSO.DeleteFile "c:\optimizer\output\current_setup.csv"

WScript.StdOut.Write("Generating Summary report 1 " & vbCrLf)
'summary report 1; with devices
sql  =	" (select 'pkgtype', 'tester_req','EDGE_LB','pkg_size','Device_Name','Package','expected_output_2_days','expected_avg_output_per_day' " & _
 ",'cap_per_day','wip_post_WB','wip_ft1','wip_ft2','POST_WB_QUEUE','TEST_QUEUE_FT1','TEST_QUEUE_FT2','Total_queue_1_tester') " & _
 " union  " & _
" (SELECT pkg.pkgtype, sol.Machine_Family_Name as tester_req, " & _
" sol.Tooling_Family_Name as EDGE_LB " & _
" ,pkg.pkg_size, sol.Device_Name , pkg.pin as package " & _
" , sum(sol.Quantity) as expected_output_2_days " & _
" , sum(sol.Quantity)/2 as expected_avg_output_per_day " & _
" , round(avg(cap.capacity),2) as cap_per_day  " & _
" , COALESCE((wip_postwb.total_wip),0) as wip_post_WB " & _
" , COALESCE((wip_ft1.total_wip),0) as wip_ft1    " & _
" , COALESCE((wip_ft2.total_wip),0) as wip_ft2 " & _
" , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE " & _
" , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1 " & _
"  , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2 " & _
" , coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0)  as total_wip " & _
" FROM solution sol " & _
" left join device_cap cap on cap.device=sol.Device_Name " & _
" left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _ 
" left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name " & _
" group by sol.package, sol.Tooling_Family_Name,sol.Machine_Family_Name,sol.Device_Name " & _
"   INTO OUTFILE 'C:/optimizer/output/optimizer_summary_report.csv'  " & _
"  	FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _
"  	LINES TERMINATED BY '\n'  " & _
")"
myrs.open sql,mycn
WScript.StdOut.Write("Generating Summary report 1 :Complete" & vbCrLf)

WScript.StdOut.Write("Generating Summary report 2 " & vbCrLf)
'summary report 2. without device
sql  =	" (select 'pkgtype', 'tester_req','EDGE_LB','pkg_size','Pin','Package','expected_output_2_days','expected_avg_output_per_day' " & _
 " ,'cap_per_day','wip_post_WB','wip_ft1','wip_ft2','POST_WB_QUEUE','TEST_QUEUE_FT1','TEST_QUEUE_FT2', 'Total_Queue_1_tester') " & _
 " union  " & _
" (SELECT pkg.pkgtype, sol.Machine_Family_Name as tester_req, " & _
" sol.Tooling_Family_Name as EDGE_LB " & _
" ,pkg.pkg_size , pkg.Pin,pkg.Pin as package " & _
" , sum(sol.Quantity) as expected_output_2_days " & _
" , sum(sol.Quantity)/2 as expected_avg_output_per_day " & _
" , round(avg(cap.capacity),2) as cap_per_day  " & _
" , COALESCE((wip_postwb.total_wip),0) as wip_post_WB " & _
" , COALESCE((wip_ft1.total_wip),0) as wip_ft1    " & _
" , COALESCE((wip_ft2.total_wip),0) as wip_ft2 " & _
" , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE " & _
" , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1 " & _
" , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2 " & _
" , coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0)  as total_wip " & _
" FROM solution sol " & _
" left join device_cap cap on cap.device=sol.Device_Name " & _
" left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
" left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name" & _
" group by sol.package, sol.Tooling_Family_Name,sol.Machine_Family_Name" & _
"   INTO OUTFILE 'C:/optimizer/output/optimizer_summary_report_no_device.csv'  " & _
"  	FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _
"  	LINES TERMINATED BY '\n'  " & _
")"
myrs.open sql,mycn
WScript.StdOut.Write("Generating Summary report 2 : complete " & vbCrLf)

WScript.StdOut.Write("Generating Summary report 3: queue by loadboard " & vbCrLf)
sql=" (select 'pkgtype','EDGE_LB','num_lots','pkg_size','expected_output_2_days','expected_avg_output_per_day','cap_per_day','wip_post_WB','wip_ft1','wip_ft2' " & _
" ,'POST_WB_QUEUE','TEST_QUEUE_FT1','TEST_QUEUE_FT2','Total_Queue_1_tester' ) " & _
" union " & _
" (SELECT pkg.pkgtype, " & _
" sol.Tooling_Family_Name as EDGE_LB, count(sol.Lot_Name) as num_lots  " & _
" ,pkg.pkg_size  " & _
" , sum(sol.Quantity) as expected_output_2_days  " & _
" , sum(sol.Quantity)/2 as expected_avg_output_per_day " & _ 
" , round(avg(cap.capacity),2) as cap_per_day  " & _
" , COALESCE((wip_postwb.total_wip),0) as wip_post_WB " & _
" , COALESCE((wip_ft1.total_wip),0) as wip_ft1    " & _
" , COALESCE((wip_ft2.total_wip),0) as wip_ft2 " & _
" , COALESCE(round((wip_postwb.total_wip)/avg(cap.capacity),2),0) as POST_WB_QUEUE " & _
" , COALESCE(round((wip_ft1.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT1 " & _
" , COALESCE(round((wip_ft2.total_wip)/avg(cap.capacity),2),0) as TEST_QUEUE_FT2 " & _
" , coalesce((COALESCE((wip_postwb.total_wip),0) + COALESCE((wip_ft1.total_wip),0) + COALESCE((wip_ft2.total_wip),0)) / round(avg(cap.capacity),2),0)  as Total_Queue_1_tester " & _
" FROM solution sol " & _
" left join device_cap cap on cap.device=sol.Device_Name " & _
" left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT in (4410) group by wip1.device) wip_ft1 on wip_ft1.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT in (4420) group by wip1.device) wip_ft2 on wip_ft2.device=sol.device_name " & _
" left join (select sum(wip1.CUR_QTY) as total_wip, wip1.Device from wip_cut wip1 where wip1.LPT not in (4410,4420) group by wip1.device) wip_postwb on wip_postwb.device=sol.Device_Name " & _
" group by sol.Tooling_Family_Name " & _
" INTO OUTFILE 'C:/optimizer/output/optimizer_queue_by_loadboard.csv'  " & _
" FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY ''  " & _
" LINES TERMINATED BY '\n' " & _
" ) " 

WScript.StdOut.Write("Generating Summary report 3 : complete " & vbCrLf)
myrs.open sql,mycn

WScript.StdOut.Write("Generating optimizer_package_report " & vbCrLf)
'optimizer_package_report
sql = "(select 'pkgtype', 'num_lots','expected_output_2_days','expected_avg_per_day','cumulative_output_per_day') " & _
"  union  " & _
"  ( " & _
"  select pkgtype, num_lots, expected_output_2_days, expected_avg_per_day " & _
"  , @cumtotal := @cumtotal + expected_avg_per_day as cumulative_output_per_day " & _
"  from ( " & _
" 	select @cumtotal := 0  " & _
" 		 ,@qtytotal := 0 " & _
" ) i " & _
"  join " & _
" (select " & _
" pkg.pkgtype,count(sol.lot_name) as num_lots,sum(sol.Quantity) as expected_output_2_days " & _
" , sum(sol.Quantity)/2 as expected_avg_per_day " & _
"  from solution sol " & _
"  left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
"  left join device_cap cap on cap.device=sol.Device_Name " & _
"  group by pkg.pkgtype " & _
"  ) aa " & _
"  INTO OUTFILE 'C:/optimizer/output/optimizer_package_report.csv'  " & _
"  	FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _
" 	LINES TERMINATED BY '\n' " & _
"  )     "
myrs.open sql,mycn
WScript.StdOut.Write("Generating optimizer_package_report: Complete " & vbCrLf)

WScript.StdOut.Write("Generating optimizer_tester_config_report " & vbCrLf)

'optimizer_tester_config_report
sql = "(select 'tester_config', 'num_lots','expected_output_2_days','expected_avg_per_day','cumulative_output_per_day') " & _
"  union (  " & _
" select apu_config, num_lots, expected_output_2_days, expected_avg_per_day " & _
"  , @cumtotal := @cumtotal + expected_avg_per_day as cumulative_output_per_day " & _
"  from ( " & _
" 	select @cumtotal := 0  " & _
" 		 ,@qtytotal := 0 " & _
" ) i " & _
"  join " & _
"  (select apu.config as apu_config, count(sol.Lot_Name) as num_lots, sum(sol.Quantity) as expected_output_2_days " & _
"  , sum(sol.Quantity)/2 as expected_avg_per_day " & _
"  from solution sol " & _
"  left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
"  group by apu.config " & _
"  ) aa" & _
"  INTO OUTFILE 'C:/optimizer/output/optimizer_tester_config_report.csv'  " & _
"  	FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _
" 	LINES TERMINATED BY '\n' " & _
"  )     "
myrs.open sql,mycn
WScript.StdOut.Write("Generating optimizer_tester_config_report : Complete " & vbCrLf)

WScript.StdOut.Write("Generating forEcast loadboard setup changes" & vbCrLf)
'forecast_loadboard_setup_changes
sql="(select 'tester_index', 'tester_config','loadboard_type','setup_change','Package','apu_tester','package_type','pin','package_size' " & _
"  ,'Lot_Name','quantity','Lot_Weight','device_name','Setup_Time','Completion_Time','Capacity') " & _
" union " & _
" (select tester_index, tester_config,loadboard_type, setup_change,Package,apu_tester,package_type,pin,package_size, " & _
" Lot_Name,quantity,Lot_Weight,device_name,Setup_Time,Completion_Time,capacity " & _
" from (	select tester_index, tester_config, loadboard_type " & _
"	,case when @prev_tester = aa.tester_config and @prev_loadboard_type = aa.loadboard_type " & _
"    then '' else if(@prev_tester is null,'', if(@prev_tester_index = tester_index,'setup change','') ) end as setup_change " & _
"	,Package,apu_tester,package_type,package_size,pin " & _
"    ,aa.Lot_Name, aa.quantity, aa.Lot_Weight  " & _
"    ,device_name,aa.Setup_Time,aa.Completion_Time,aa.capacity " & _
"	,(@prev_tester := aa.tester_config) as temp_tester_config " & _
"	,(@prev_loadboard_type := aa.loadboard_type) as temp_loadboard_config " & _
"    ,(@prev_tester_index := aa.tester_index) " & _
"	from ( " & _
"		select @prev_tester := null   " & _
"			 ,@prev_loadboard := null   " & _
"             ,@prev_tester_index := null " & _
"	) i " & _
"	join ( " & _
"	select trim(right(sol.Machine_Instance,2)) as tester_index " & _
"	, sol.Machine_Family_Name as tester_config " & _
"	, sol.Tooling_Family_Name as loadboard_type " & _
"	, sol.Package " & _
"	, apu.tester as apu_tester " & _
"	, pkg.pkgtype as package_type " & _
"	, pkg.pkg_size as package_size " & _
"	, sol.device_name " & _
"	, Completion_Time " & _
"   , pkg.pin, sol.lot_name, sol.Quantity,sol.Lot_Weight,sol.Setup_Time,cap.capacity " & _
"	from  " & _
"	solution sol " & _
"	left join device_cap cap on cap.device=sol.Device_Name " & _
"	left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
"	left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
"	order by  trim(right(sol.Machine_Instance,2)),sol.Completion_Time " & _
"	) aa " & _
"	order by tester_index,Completion_Time)bb " & _
"   INTO OUTFILE 'C:/optimizer/output/optimizer_loadboard_setup_change.csv'   " & _
" FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY ''  " & _
" LINES TERMINATED BY '\n'  " & _
" )"
'myrs.open sql,mycn

'forecast_loadboard_setup_changes
sql1=" select tester_index, tester_config,loadboard_type, setup_change,Package,apu_tester,package_type,package_size, " & _
" device_name,Completion_Time " & _
" from (	select tester_index, tester_config, loadboard_type " & _
"	,case when @prev_tester = aa.tester_config and @prev_loadboard_type = aa.loadboard_type  " & _
"    then '' else if(@prev_tester is null,'', if(@prev_tester_index = tester_index,'setup change','') ) end as setup_change " & _
"	,Package,apu_tester,package_type,package_size,pin " & _
"    ,aa.Lot_Name, aa.quantity, aa.Lot_Weight  " & _
"    ,device_name,aa.Setup_Time,aa.Completion_Time,aa.capacity " & _
"	,@prev_tester := aa.tester_config as temp_tester_config " & _
"	,@prev_loadboard_type := aa.loadboard_type as temp_loadboard_config " & _
"    ,@prev_tester_index := aa.tester_index " & _
"	from ( " & _
"		select @prev_tester := null   " & _
"			 ,@prev_loadboard := null   " & _
"             ,@prev_tester_index := null " & _
"	) i " & _
"	join ( " & _
"	select trim(right(sol.Machine_Instance,2)) as tester_index " & _
"	, sol.Machine_Family_Name as tester_config " & _
"	, sol.Tooling_Family_Name as loadboard_type " & _
"	, sol.Package " & _
"	, apu.tester as apu_tester " & _
"	, pkg.pkgtype as package_type " & _
"	, pkg.pkg_size as package_size " & _
"	, sol.device_name " & _
"	, Completion_Time " & _
 "   , pkg.pin, sol.lot_name, sol.Quantity,sol.Lot_Weight,sol.Setup_Time,cap.capacity " & _
"	from  " & _
"	solution sol " & _
"	left join device_cap cap on cap.device=sol.Device_Name " & _
"	left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
"	left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
"	order by  trim(right(sol.Machine_Instance,2)),sol.Completion_Time " & _
"	) aa " & _
"	order by tester_index,Completion_Time)bb"

	Dim oRs: Set oRs = CreateObject("ADODB.Recordset")
	oRs.CommandType = adCmdText
	oRs.CursorLocation=adUseClient
	oRs.CursorType=adOpenDynamic
	oRs.LockType=adLockBatchOptimistic
	Set oRs = mycn.Execute(sql1)
	
	
	if not oRs.eof and not oRs.bof  then
		if objFSO.FileExists("c:\optimizer\output\optimizer_loadboard_setup_change.csv")   then
			Set objFilek = objFSO.OpenTextFile("c:\optimizer\output\optimizer_loadboard_setup_change.csv", 2, True)
		else
			Set objFilek = objFSO.CreateTextFile("c:\optimizer\output\optimizer_loadboard_setup_change.csv",True)
		end if
		
		objFilek.Close
		strNewContent1 = strNewContent1 & "tester_index, tester_config,loadboard_type, setup_change,Package,apu_tester,package_type,package_size,device_name,Completion_Time" & vbCrLf
		oRs.movefirst
		WScript.StdOut.Write("Generating loadboard csv report" & vbCrLf)
		Do Until  oRs.EOF
		WScript.StdOut.Write(oRs.Fields(0).Value & vbCrLf)
			strNewContent1 = strNewContent1 & """" & oRs.Fields(0).Value & """" & "," & """" & oRs.Fields(1).Value & """" & "," & """" & oRs.Fields(2).Value & """" & "," & """" & oRs.Fields(3).Value & _
				"""," & """" & oRs.Fields(4).Value & """" & "," & """" & oRs.Fields(5).Value & """" & "," & """" & oRs.Fields(6).Value & """" & ","  & """" & oRs.Fields(7).Value & """" & "," & """" & oRs.Fields(8).Value  & _
				"""" & "," & """" & oRs.Fields(9).Value & """" & vbCrLf 
			oRs.MoveNext
			'WScript.StdOut.Write(oRs.Fields(0).Value & vbCrLf)
			'echo strNewContent1
		loop
	
		Set objFilek = objFSO.OpenTextFile("c:\optimizer\output\optimizer_loadboard_setup_change.csv",ForWriting) ' remove all contents
		objFilek.Write strNewContent1 ' write key devices to keydevices.csv data
		objFilek.Close	
	end if
	
	Set oRs = Nothing
	

WScript.StdOut.Write("Generating detail report" & vbCrLf)
' optimizer raw data
sql=" (select 'tester_index', 'tester_config','loadboard_type','setup_change','Package','apu_tester','package_type','pin','package_size' " & _
" ,'Lot_Name','quantity','Lot_Weight','device_name','Setup_Time','Completion_Time','capacity') " & _
" union ( select tester_index, tester_config,loadboard_type, setup_change,Package,apu_tester,package_type,pin,package_size, " & _
" Lot_Name,quantity,Lot_Weight,device_name,Setup_Time,Completion_Time,capacity " & _
"  from ( 	select tester_index, tester_config, loadboard_type " & _
	" ,case when @prev_tester = aa.tester_config and @prev_loadboard_type = aa.loadboard_type " & _
"     then '' else if(@prev_tester is null,'', if(@prev_tester_index = tester_index,'setup change','') ) end as setup_change " & _
" 	,Package,apu_tester,package_type,package_size,pin " & _
"     ,aa.Lot_Name, aa.quantity, aa.Lot_Weight  " & _
"     ,device_name,aa.Setup_Time,aa.Completion_Time,aa.capacity " & _
	" ,(@prev_tester := aa.tester_config) as temp_tester_config " & _
"  	,(@prev_loadboard_type := aa.loadboard_type) as temp_loadboard_config " & _
"     ,(@prev_tester_index := aa.tester_index) " & _
"  	from ( " & _
"  		select @prev_tester := null  " & _
" 			 ,@prev_loadboard := null  " & _
"               ,@prev_tester_index := null " & _
"  	) i " & _
"  	join ( " & _
" 	select trim(right(sol.Machine_Instance,2)) as tester_index " & _
"  	, sol.Machine_Family_Name as tester_config " & _
"  	, sol.Tooling_Family_Name as loadboard_type " & _
" 	, sol.Package " & _
"  	, apu.tester as apu_tester " & _
"  	, pkg.pkgtype as package_type " & _
" 	, pkg.pkg_size as package_size " & _
"  	, sol.device_name " & _
"  	, Completion_Time " & _
"     , pkg.pin, sol.lot_name, sol.Quantity,sol.Lot_Weight,sol.Setup_Time,cap.capacity " & _
"  	from " & _
"  	solution sol " & _
"	left join device_cap cap on cap.device=sol.Device_Name " & _
"	left join (select wip.lot_name,wip.package as package ,pkg.pkgtype,pkg.pkg_grp,wip.pin as pin, wip.Package as pkg_size from wip wip " & _ 
" inner join package_size pkg on pkg.package=wip.Pin and upper(pkg.pkg_size) = upper(wip.package) " & _
" group by wip.lot_name,wip.pin,pkg.pkgtype,pkg.pkg_grp,wip.package) pkg on pkg.package=sol.Package and pkg.lot_name = sol.Lot_Name " & _
"	left join (SELECT trim(right(apu.tester,2)) as tester, config FROM apu) apu on apu.tester = trim(right(sol.Machine_Instance,2)) " & _
"	order by  trim(right(sol.Machine_Instance,2)),sol.Completion_Time" & _
"  ) aa " & _
"  )bb " & _
"  group by tester_index, tester_config, loadboard_type,setup_change,Package,apu_tester,package_type,package_size,device_name,Completion_Time " & _
"  order by tester_index,Completion_Time " & _
"  INTO OUTFILE 'C:/optimizer/output/optimizer_detailed_report.csv'  " & _
"  FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _
"  LINES TERMINATED BY '\n' " & _
" ) "
myrs.open sql,mycn
WScript.StdOut.Write("Generating detail report : Complete" & vbCrLf)

'generate device log points
WScript.StdOut.Write("Generating device logpoint report" & vbCrLf)
sql= "(select 'device', 'logpoint','quantity','average_capacity','Queue') " & _
" union( " & _
" select wip.device,wip.LPT,sum(wip.CUR_QTY) as Quantity,round(avg(cap.capacity),2) as average_capacity, coalesce((round(sum(wip.CUR_QTY)/round(avg(cap.capacity),2),2)),0) as Queue  from wip wip " & _
" left join device_cap cap on cap.device=wip.Device " & _
" group by wip.Device,wip.LPT " & _
" INTO OUTFILE 'C:/optimizer/output/wip_logpoint.csv' " & _
"FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY '' " & _ 
" LINES TERMINATED BY '\n')"
myrs.open sql,mycn
WScript.StdOut.Write("Generating device logpoint report : Complete" & vbCrLf)

WScript.StdOut.Write("Generating Current Setup" & vbCrLf)
'generate current_setup
sql= "(select 'loadboard','device_item_id','pin','temperature','tester_config','tester','tester_id','package') " & _
" union( " & _
" select case when cl.aux_type_TWSTATUS_i = '' then cl.hib_type_TWSTATUS_i else cl.aux_type_TWSTATUS_i end as loadboard,  " & _
" cl.devid_LOT_r as device_item_id, cl.pinOnTester_f as pin, substr(cl.tempOnTstr_f,6,2) as temperature,  " & _
" cl.testerConfig_CMMSTESTERS_i as tester_config, cl.tester_TWSTATUS_i as tester, " & _
" case when substr(tester_TWSTATUS_i,1,4)= 'TT55' then substr(cl.tester_TWSTATUS_i,5,2) else '' end as tester_id " & _
" ,substr(cl.pkgOnTstr_f,1,3) as package " & _
" from clattester_stationstatus cl " & _
" where substr(cl.tester_TWSTATUS_i,1,4)= 'TT55' " & _
" and tempOnTstr_f<>'' " & _
" INTO OUTFILE 'C:/optimizer/output/current_setup.csv'  " & _
" FIELDS ENCLOSED BY '""' TERMINATED BY ',' ESCAPED BY ''  " & _
" LINES TERMINATED BY '\n' " & _
" )"
myrs.open sql,mycn
WScript.StdOut.Write("Generating Current Setup: Complete" & vbCrLf)

mycn.close
set mycn = nothing
set myrs = nothing