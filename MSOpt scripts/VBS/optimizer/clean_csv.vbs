'#########################################################
'Clean csv files
'AUTH: Michael
'VERSION: 1.3
'DESC:
'	Scripts to be run before executing AT.exe
'
' -Download keydevices
' -Remove  multiple space at WIP.csv start_time field
' -Read CSV File
'##########################################################
'change sql here
Dim sql : sql = "select DEVICE_NAME,TARGET_AMOUNT from rpttest_admin.optimizer_key_devices" 

'##########################################################

Const ForReading = 1
Const ForWriting = 2
	
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\optimizer\data\wip.csv", ForReading)

dim local_input : local_input = "c:\optimizer\data"

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    arrFields = Split(strLine, ",")
	' Check if field has spaces
	if left(arrFields(12),2) = "  " then 
		arrFields(12) = replace(arrFields(12)," ", "")
	end if
    strNewContent = strNewContent & arrFields(0) & "," & arrFields(1) & _ 
					"," & arrFields(2) & "," & arrFields(3) & "," & arrFields(4) & "," & arrFields(5) & _
					"," & arrFields(6) & "," & arrFields(7) & "," & arrFields(8) & "," & arrFields(9) & _
					"," & arrFields(10) & "," & arrFields(11) & "," & arrFields(12) & "," & arrFields(13) & _ 
					"," & arrFields(14) & vbCrLf
Loop

objFile.Close

Set objFile = objFSO.OpenTextFile("C:\optimizer\data\wip.csv",ForWriting)

objFile.Write strNewContent

objFile.Close

Download_input_files

Sub Download_input_files()

	 ' download directly from database keydevices
	on error resume next
	Dim strCon
	strCon ="Provider=OraOLEDB.Oracle;Data Source=CLARK_MSOPT;User ID=""rpt_user"";Password=""rptuser1"""

	Dim oCon: Set oCon = CreateObject("ADODB.Connection")
	Dim oRs: Set oRs = CreateObject("ADODB.Recordset")
	oCon.Open strCon
	oRs.CommandType = adCmdText
	oRs.CursorLocation=adUseClient
	oRs.CursorType=adOpenDynamic
	oRs.LockType=adLockBatchOptimistic

	'change sql statement here;
	Set oRs = oCon.Execute(sql)
	
	if not oRs.eof and not oRs.bof  then
		if objFSO.FileExists(local_input & "\keydevices.csv")   then
			Set objFilek = objFSO.OpenTextFile(local_input & "\keydevices.csv", 2, True)
		else
			Set objFilek = objFSO.CreateTextFile(local_input & "\keydevices.csv",True)
		end if
		
		objFilek.Close
		strNewContent1 = strNewContent1 & "Key_Device,TARGET" & vbCrLf
		oRs.movefirst
		Do Until  oRs.EOF
			strNewContent1 = strNewContent1 & oRs.Fields(0).Value & "," & oRs.Fields(1).Value & vbCrLf
			oRs.MoveNext
			'echo strNewContent1
		loop
	
		Set objFilek = objFSO.OpenTextFile(local_input & "\keydevices.csv",ForWriting) ' remove all contents
		objFilek.Write strNewContent1 ' write key devices to keydevices.csv data
		objFilek.Close	
	end if
	oCon.Close
	Set oRs = Nothing
	Set oCon = Nothing
end sub