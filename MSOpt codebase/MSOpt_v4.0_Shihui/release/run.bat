REM WorkAround on Absoulute path error of BCP to writing to files
@echo off
REM Set  current_directory=%~dp0

cd %~dp0
/release/at_Shuhui.exe
