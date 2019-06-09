'#########################################################
'read csv into array
'AUTH: Neil
'VERSION: 1.0
'DESC:
'	Scripts to be run after executing AT.exe
'##########################################################
' FIELDS: 
' 	long_lot:= Lot Name, Device, Key Device, Key Package, Key Pin Package, Num of Device, Remarks
' 	keydevice_production:= id, device, target_amount, finished_amount, shortage
'
' CONDITIONS:
' 	Treshold = 1.1 (10%)
'##########################################################
'
' -Query the following data: keydevice_production; optimizer_summary_report
' -Check and verify if the keydevice_production irregularity
' -Read CSV file to Array
' -Perform verification
'##########################################################

Const ForReading = 1
Const ForWriting = 2
Const Treshhold = 1.1

dim local_input : local_input = "c:\temp"
dim llot_cnt : llot_cnt = 0
dim check_cnt : check_cnt = 0

' read long_lot.csv	
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\temp\long_lot.csv", ForReading)

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    arrFields = Split(strLine, ",")
	llot_cnt = llot_cnt + 1
Loop

'wscript.echo nRow
objFile.Close
	
set oCn = CreateObject("ADODB.Connection")
set oRs = CreateObject("ADODB.Recordset")
'connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=ccle0174881;Database=optimizer;User=root; Password=root;"
connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=localhost;Database=optimizer;User=root; Password=;"

' open connectionstring
wscript.echo "open connection..."
oCn.Open connectionString

' build SQL script
sql = "SELECT * FROM optimizer.keydevice_production"

' open recordset
wscript.echo "open recordset..."
'oRs.Open sql, oCn, adOpenDynamic, adLockPessimistic, adCmdText
oRs.Open sql, oCn, 2, 2, 1

' loop in the recordset and check if device is in the long_lot, insert into arrCheck
Do While Not oRs.EOF
	For r = 1 to llot_cnt
		if oRs(1).value = arrFields(r) then
			arrCheck(check_cnt) = oRs(1).value
			check_cnt = check_cnt + 1
		end if
	Next
	oRs.MoveNext
Loop

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
" group by sol.package, sol.Tooling_Family_Name,sol.Machine_Family_Name,sol.Device_Name"

'wscript.echo check_cnt

' close connectionstring
wscript.echo "close connection..."
oCn.close

' cleanup recordset & connection
wscript.echo "set nothing to recordset..."
set oRs = nothing
set oCn = nothing
