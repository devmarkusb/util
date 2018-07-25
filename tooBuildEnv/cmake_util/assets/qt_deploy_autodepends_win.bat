REM Automatically fills the bin dir with full Qt and compiler dependencies of the app exe (for Windows).
REM Expects param 1 to be the full path to the binary build dir, param 2 to be the full path to windeployqt,
REM and the remaining params to be all the designated arguments for windeployqt.

@echo off
set bindir=%1
echo cd %bindir%
cd %bindir%

set windeploypath=%2

shift
shift
set params=%1
:loop
shift
if [%1]==[] goto afterloop
set params=%params% %1
goto loop
:afterloop


echo %windeploypath%/windeployqt %params%
%windeploypath%/windeployqt %params%
