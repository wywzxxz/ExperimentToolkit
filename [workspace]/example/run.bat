@echo off
cd %~dp0
call ..\..\[testModule]\testModule.bat ^
dist ^
source\_test\statics ^
source\_test\input\input.exe ^
source\_test\output\output.exe ^
source\A\main.exe ^
source\B\main.exe ^