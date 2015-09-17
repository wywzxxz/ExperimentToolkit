setlocal enabledelayedexpansion
set statics=%1%

echo ¾²Ì¬ÄÚÈÝ:		.\statics
cd build
echo ¸´ÖÆstatics
xcopy ..\statics\* . /s /h /y
xcopy ..\!statics!\* . /s /h /y

start /i nw.exe .