'#########################################################
'read route csv into array plus 500 to count
'AUTH: Neil
'VERSION: 1.0
'DESC:
'	Read route.csv then count the rows, update line 7 of input.txt of value route.csv
'##########################################################

Const ForReading = 1
Const ForWriting = 2
dim nRow : nRow = 0

Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile("c:\temp\route.csv", ForReading)

dim local_input : local_input = "c:\temp"

Do Until objFile.AtEndOfStream
    strLine = objFile.ReadLine
    arrFields = Split(strLine, ",")
	nRow = nRow + 1
Loop

dim nCnt : nCnt = 0
Set objFileToRead = CreateObject("Scripting.FileSystemObject").OpenTextFile("C:\temp\input.txt",ForReading)

if objFSO.FileExists("c:\temp\input1.txt")   then
	Set objFilek = objFSO.OpenTextFile("c:\temp\input1.txt", ForWriting, True)
else
	Set objFilek = objFSO.CreateTextFile("c:\temp\input.txt",True)
end if

do while not objFileToRead.AtEndOfStream
     strLine = objFileToRead.ReadLine()
     'Do something with the line
	 arrInput = Split(strLine,"//")
	 nCnt = nCnt + 1

	' store data in string before writing it to text file
	if nCnt = 7 then	
		strNewContent = strNewContent & nRow & " //" & arrInput(1) & vbCrLf
	else
		strNewContent = strNewContent & arrInput(0) & " //" & arrInput(1) & vbCrLf
	end if
loop

objFilek.Write strNewContent ' write new input.txt  new value of line 7
objFilek.Close

wscript.echo "close objFileToRead"
objFileToRead.Close
Set objFileToRead = Nothing
objFile.Close