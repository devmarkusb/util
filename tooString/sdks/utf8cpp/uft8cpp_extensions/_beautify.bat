@echo off
setlocal
:PROMPT
SET /P AREYOUSURE=Are you sure (Y/[N])?
IF /I "%AREYOUSURE%" NEQ "Y" GOTO END

@echo on
for /r %%f in (*.cpp,*.c,*.cc,*.h,*.hpp) do clang-format -i -style=file "%%f"
@echo off

:END
endlocal
