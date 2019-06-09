' @brief using ADODB in VBScript
' MySQL connector: MySQL ODBC 5.1

' connection and recordset variables
'Dim rs, strConnectString, strSQL
'dim cn, rs
dim arrRows

set mycn = CreateObject("ADODB.Connection")
set myrs = CreateObject("ADODB.Recordset")
'connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=ccle0174881;Database=optimizer;User=root; Password=root;"
connectionString = "Driver={MySQL ODBC 5.1 Driver};Server=localhost;Database=optimizer;User=root; Password=;"

' open connectionstring
wscript.echo "open connection..."
mycn.Open connectionString

' open recordset
wscript.echo "open recordset..."
'myrs.Open "select * from apu", mycn, adOpenDynamic, adLockPessimistic, adCmdText
myrs.Open "select * from apu", mycn, 2, 2, 1

' loop in the recordset and print it on screen
Do While Not myrs.EOF
	wscript.echo myrs(0).value & " - " & myrs(1).value
	myrs.MoveNext
Loop

myrs.MoveFirst()
' load recordset to array
arrRows = myrs.GetRows()

' print number of records retrieved.
wscript.echo UBound(arrRows, 2) + 1 & " records retrieved."
wscript.echo UBound(arrRows, 1) + 1 & " columns retrieved."

' loop in the array and print all the records on screen
For iRow = 0 to UBound(arrRows,2)
	For iCol = 0 to UBound(arrRows,1)
		wscript.echo arrRows(iCol,iRow)
	Next 'iCol
Next 'iRow

' close connectionstring
wscript.echo "close connection..."
mycn.close

' cleanup recordset & connection
wscript.echo "set nothing to recordset..."
set myrs = nothing
set mycn = nothing
