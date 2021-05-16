@echo off
setlocal enableextensions
cd /d "%~dp0"

:StartLoop
if  "%~1"== "" goto EndLoop
recolorHelper.exe "%~1" echo on
shift
goto StartLoop
:EndLoop
PAUSE