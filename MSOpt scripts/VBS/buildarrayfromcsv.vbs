
Function BuildArrayFromCsv(filepath)    'Function to read file and load into an array

Const ForReading = 1    ' Declare constant for reading for more clarity

Set objFSO = CreateObject("Scripting.FileSystemObject")

' open csv file for reading
wscript.echo "Open csv file for reading..." 
Set inputFile = objFSO.OpenTextFile(filepath, ForReading, True) ' Set inputFile as file to be read from

Dim row, column
Dim fields(4) '12 fields per line  
inputFile.ReadAll 'read to end of file  

ReDim MyArray(4,inputFile.Line-2) 'current line, minus one for header, and minus one for starting at zero  
inputFile.close     'close file so that MyArray can be filled with data starting at the top
Set inputFile = objFSO.OpenTextFile(filepath, ForReading, True) 'back at top  
inputFile.ReadLine 'skip header 

Do Until inputFile.AtEndOfStream  
    fields = Split(inputFile.Readline,",") 'store line in temp array  
    'For column = 0 To 4 'iterate through the fields of the temp array  
    '    myArray(row,column) = fields(column) 'store each field in the 2D array with the given coordinates  
    'Next
    row = row + 1  'next line 
Loop

'wscript.echo row

' close csv file
wscript.echo "close csv file..."
inputFile.close
End Function

BuildArrayFromCsv("c:\temp\keydevice_production.csv")