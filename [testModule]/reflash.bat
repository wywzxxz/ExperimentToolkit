setlocal enabledelayedexpansion
set statics=%1%

echo ��̬����:		.\statics
cd build
echo ����statics
xcopy ..\statics\* . /s /h /y
xcopy ..\!statics!\* . /s /h /y

start /i nw.exe .