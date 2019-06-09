'#########################################################
'read csv into array
'AUTH: Neil
'VERSION: 1.0
'DESC:
'	Scripts to be run after executing AT.exe
'##########################################################
' FIELDS: Device, Target amount, Finished amount, Shortage
'##########################################################
'
' -Download the following csv files: keydevice_production; optimizer_summary_report
' -Check and verify if the keydevice_production irregularity
' -Read CSV file to Array
' -Perform verification
'##########################################################

Const ForReading = 1
Const ForWriting = 2
Dim nRow : nRow = 0
	
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\temp\keydevice_production.csv", ForReading)

dim local_input : local_input = "c:\temp"

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    arrFields = Split(strLine, ",")
	' Check if field has spaces
'	if left(arrFields(12),2) = "  " then 
'		arrFields(12) = replace(arrFields(12)," ", "")
'	end if

'    strNewContent = strNewContent & arrFields(0) & "," & arrFields(1) & _ 
'					"," & arrFields(2) & "," & arrFields(3) & "," & arrFields(4) & "," & arrFields(5) & _
'					"," & arrFields(6) & "," & arrFields(7) & "," & arrFields(8) & "," & arrFields(9) & _
'					"," & arrFields(10) & "," & arrFields(11) & "," & arrFields(12) & "," & arrFields(13) & _ 
'					"," & arrFields(14) & vbCrLf

	nRow = nRow + 1
Loop

wscript.echo nRow

objFile.Close