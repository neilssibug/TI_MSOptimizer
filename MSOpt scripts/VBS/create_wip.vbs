'generate WIP
'MOD: Mike
'Date: 2015-09-11
'DESC: Generate report
'##################################################################################
Const ForReading = 1
Const ForWriting = 2
'parameters
dim local_folder : local_folder = "c:\optimizer"

on error resume next
Dim folder 
Dim strCon
set mycn = CreateObject("ADODB.Connection")
set myrs = CreateObject("ADODB.Recordset")
connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=ccle0174881;Database=optimizer;User=root; Password=root;"
'connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=localhost;Database=optimizer;User=root; Password=root;"
mycn.Open connectionString

Set objFSO = CreateObject("Scripting.FileSystemObject")

'delete existing files.
objFSO.DeleteFile "c:\optimizer\output\optimizer_summary_report.csv"

sql1= " select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,'' as START_TIME,SNAPSHOOT_TIME,'' as MACHINE_INSTANCE from ( " & _
" select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT, " & _
" CUM_PLAN_CT,LOT_AGE,'' as START_TIME,SNAPSHOOT_TIME,'' as MACHINE_INSTANCE,wipdays,ID, " & _
" case when keydev=1 and key_total>(kd.Target * 1.1 ) and one_dev=0 then 9999 when keydev=1 then 0.1 else round(cumwipdays,2) end as cumwipdays,key_total,tempdev,keydev,target * 1.1 as target  " & _
" from ( " & _
	" select a.LOT,a.DEVICE,a.PACKAGE,a.PIN,a.CUR_QTY,a.WEIGHT,a.LOT_NAME,a.LPT,a.OPN,a.TOTAL_PLAN_CT,a.CUM_PLAN_CT,a.LOT_AGE,'' as START_TIME,a.SNAPSHOOT_TIME,'' as MACHINE_INSTANCE,a.wipdays,a.ID  " & _
"		 ,case when @prev_device = a.device then @cumtotal := @cumtotal + a.wipdays else @cumtotal := a.wipdays end as cumwipdays  " & _
"		 ,case when @prev_device = a.device and a.keydev=1 then @qtytotal := @qtytotal + a.CUR_QTY else @qtytotal := a.CUR_QTY end as key_total  " & _
" 		 ,(@prev_device := a.device) as tempdev  " & _
"		 , a.keydev " & _
"		, case when w1.device=a.device and w1.id=a.id then 1 else 0 end as one_dev " & _
" 	 from ( " & _
		 " select @prev_device := null  " & _
		 " ,@cumtotal := 0  " & _
		 " ,@qtytotal := 0 " & _
	 " ) i  " & _
	 " join (  " & _
	 " select LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,  " & _
	 " case when left(START_TIME,2)= ' '  then replace(START_TIME,' ' ,'' ) else START_TIME end  as START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE,wipdays,ID,keydev from  " & _
	" (  " & _
		 " select w2.*,result2.wipdays,keydev from  " & _
		 " wip w2 left join  " & _
		 " (  " & _
			 " select result1.*, result1.initial_cumu as wipdays from   " & _
			 " (  " & _
			 " select  " & _
			 " wip.*,round(wip.cur_qty / (r.pph * 24),2) as initial_cumu  " & _
			 " , case when wip.device in (select key_device from keydevices k) then 1 else 0 end as keydev " & _
			 " from wip  " & _
			 " LEFT join route r on r.Device=wip.device  " & _
			 " ) result1  " & _
			 " group by result1.device  " & _
			" order by device,weight desc, id asc " & _
		 " ) result2  " & _
		 " on w2.device = result2.device  " & _
	 " ) aa  " & _
	  " order by device,weight desc ,id asc " & _
	 " ) a " & _
	" left join (select device,min(id) as id from wip w2 group by w2.device) w1 on w1.device=a.device and w1.id=a.id " & _
" )fr1 left join keydevices kd on fr1.device=kd.key_device " & _
" order by ID " & _
" )fn " & _
" where cumwipdays<>9999 " & _
" order by ID " 



	oRs.CommandType = adCmdText
	oRs.CursorLocation=adUseClient
	oRs.CursorType=adOpenDynamic
	oRs.LockType=adLockBatchOptimistic
	Set oRs = mycn.Execute(sql1)
	
	
	if not oRs.eof and not oRs.bof  then
		if objFSO.FileExists("c:\optimizer\data\wip.csv")   then
			Set objFilek = objFSO.OpenTextFile("c:\optimizer\data\wip.csv", 2, True)
		else
			Set objFilek = objFSO.CreateTextFile("c:\optimizer\data\wip.csv",True)
		end if
		
		objFilek.Close
		strNewContent1 = strNewContent1 & "LOT,DEVICE,PACKAGE,PIN,CUR_QTY,WEIGHT,LOT_NAME,LPT,OPN,TOTAL_PLAN_CT,CUM_PLAN_CT,LOT_AGE,START_TIME,SNAPSHOOT_TIME,MACHINE_INSTANCE" & vbCrLf
		oRs.movefirst
		WScript.StdOut.Write("Generating new wip.csv file" & vbCrLf)
		Do Until  oRs.EOF
		
			strNewContent1 = strNewContent1 & """" &   oRs.Fields(0).Value & """" & "," & """" &   oRs.Fields(1).Value & """" & "," & """" &   oRs.Fields(2).Value & """" & "," & """" &   oRs.Fields(3).Value & _
				"""," & """" &   oRs.Fields(4).Value & """" & "," & """" &   oRs.Fields(5).Value & """" & "," & """" &   oRs.Fields(6).Value & """" & ","  & """" &   oRs.Fields(7).Value & """" & "," & """" &   oRs.Fields(8).Value  & _
				"""" & "," & """" &   oRs.Fields(9).Value & """" & "," & """" &   oRs.Fields(10).Value & """" & "," & """" &   oRs.Fields(11).Value & """" & "," & """" &   oRs.Fields(12).Value & _
				"""" & "," & """" &   oRs.Fields(13).Value & """" & "," & """" &   oRs.Fields(14).Value & """" & vbCrLf 
			  oRs.MoveNext
			WScript.StdOut.Write( "adding Lot: " & oRs.Fields(0).Value & vbCrLf)
			echo strNewContent1
		loop
	
		Set objFilek = objFSO.OpenTextFile("c:\optimizer\data\wip.csv",ForWriting) ' remove all contents
		objFilek.Write strNewContent1 ' write key devices to keydevices.csv data
		objFilek.Close	
	end if
	
	Set oRs = Nothing

mycn.close
set mycn = nothing
set myrs = nothing