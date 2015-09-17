setlocal enabledelayedexpansion
set wcd=%cd%
set argsnum=4
set buildpath=%wcd%\%1%
set   statics=%wcd%\%2%
set     input=%wcd%\%3%
set    output=%wcd%\%4%

set build=%buildpath%\build
set   Gstatics=%~dp0\[statics]
set   inputcfg=%buildpath%\build\inputcfg.json
set    loopcfg=%buildpath%\build\tmp\loopcfg.json
set resultfile=%buildpath%\build\result.partjson

echo build  :!build!
echo static :%Gstatics%
echo        :!statics!
echo Inputer:!input!
echo Judger :!output!

set /a i=0-!argsnum!
for %%x in (%*) do  (
	CD %buildpath%\build
	set /a i=1+!i!
	if !i! GTR 0 (
		echo                 No.!i!     %%x
	)
)
:HEAD
cd %buildpath%
call :ASKMODE 
rem start /b %~dp0\gets.bat
:mainloop
cd %buildpath%/build
if not defined seed (set seed=-1)
set /a seed=!seed!+1
echo ############################ No.!seed!
rem 创建临时文件夹
call :recreateFolder tmp & cd tmp

rem 生成输入数据
echo createing data...
call :gettime
!input! !seed! "!inputcfg!" > !buildpath!\build\input.txt
set el=!errorlevel!
call :gettime
if !el!==-1  (echo "	....HALT   .  !dtime!ms" & call :MSG "INPUT CALL TO STOP :: JOB DONE" & (start /i ..\\nw.exe .) & goto :END ) 
if !el! GEQ 1  (call :ERROR "INPUTer CRASHED with !el!" & goto :END)
if !el! LSS -1 (call :ERROR "INPUTer CRASHED with !el!" & goto :END)
echo 	....OK     . !dtime!ms

rem   主循环
set /a n=0-!argsnum!
for %%x in (%*) do  set /a n=!n!+1
set /a i=0-!argsnum!
for %%x in (%*) do  (
	CD %buildpath%\build
	set /a i=1+!i!
	if !i! GTR 0 (
		cd %build%
		echo runing %%x
		rem  -----运行结果备份 & 重建临时文件夹
		copy /y output.txt !build!\oldoutput.txt 1>nul 2>nul	
		call :recreateFolder tmp & cd tmp		
		rem  -----运行
		call :gettime		
		%wcd%\%%x !loopcfg! !cd!\..\input.txt <..\input.txt >output.txt		
		set el=!errorlevel!
		call :gettime		
		rem  -----判断			
		!output! "!inputcfg!" "!loopcfg!" "!resultfile!" !dtime! !el! "%wcd%\%%x" <output.txt
		set el=!errorlevel!
		rem -1表示自行判断
		if !el!==-1 if !n! LEQ 1  (echo "can not judge. judger require fc files but ther is only one file" & goto :END)
		if !el!==-1 if !i! GTR 1  (
			fc ..\\oldoutput.txt output.txt 1>nul 2>nul
			set el=!errorlevel!
			if !el!==2 set el=12
			if !el!==-1 set el=-10
			if !el!==1 (
				fc !build!\oldoutput.txt output.txt 
				start notepad "!build!\\oldoutput.txt"
				start notepad "!build!\\tmp\\output.txt"
				echo "|||||||[BAT COMPARER(fc):JUDEGR call to stop if the file is different]||||||||"			
				set el=2
			)			
		)
		if !el! GEQ 3  (call :ERROR "JUDEGR CRASHED with !el!" & goto :END)		
		if !el!==2  (echo "	....HALT   .  !dtime!ms" & call :ERROR "JUDGER CALL TO STOP" & goto :END)
		if !el!==1  (echo "	....FAIL   .  !dtime!ms") 
		if !el!==0  (echo 	....SUCCESS. !dtime!ms)	
		if !el!==-1  (echo 	....WAIT_FOR_COMPARE. !dtime!ms)		
		if !el! LEQ -2  (call :ERROR "JUDEGR CRASHED with !el!" & goto :END)
	)
)
rem 运行完毕，保存运行状态
echo !seed! > testModuleConfig
if not exist !build!\tmp\gets_tmp.txt  goto :mainloop
del !build!\tmp\gets_tmp.txt
goto :GOODEND
:gettime
if not defined etime (set etime=%time%)
set stime=%etime%
set etime=%time%
for /f "tokens=1,2,3,4 delims=.: " %%a in ("!stime!") do (
	set stime1=%%a & call :ee stime1
	set stime2=%%b & call :ee stime2
	set stime3=%%c & call :ee stime3 
	set stime4=%%d & call :ee stime4
)
for /f "tokens=1,2,3,4 delims=.: " %%a in ("!etime!") do (
	set etime1=%%a & call :ee etime1
	set etime2=%%b & call :ee etime2
	set etime3=%%c & call :ee etime3
	set etime4=%%d & call :ee etime4
)
set /a dtime=(((!etime1!-!stime1!)*360000+(!etime2!-!stime2!)*6000+(!etime3!-!stime3!)*100+(!etime4!-!stime4!))*10)
goto :EOF
:ee %1
set tt=%1
set temp=!%1!
if %temp:~0,1%==0 set %tt%=%temp:~1%
:recreateFolder %1
if exist %1 1>nul 2>nul (	
	rd /s/q %1 1>nul 2>nul || ( call :ERROR "recreate(rd) %1" & goto :EOF)
)
md %1 1>nul 2>nul || ( call :ERROR "recreate(md) %1" & goto :EOF)
goto :EOF
:ERROR %1
set msg=%1
echo +===============================================+
echo ^| ERROR WHEN :!msg!                                 
echo +===============================================+
pause
goto :EOF
:MSG %1
set msg=%1
echo +===============================================+
echo ^| MSG :!msg!                                 
echo +===============================================+
goto :EOF
:ASKMODE %1	
	cd %buildpath%
	if not defined mode (set mode=patch)
	set /p mode=请输入模式new/continue/patch/fullpatch.回车默认为!mode!:
	if "%mode%"=="fullpatch" (
		call %~dp0/copyGstatics
		set mode=patch)	
	if "%mode%"=="patch" (
		call %~dp0/copystatics				
		goto :GOODEND
		rem set mode=continue
	)
	if "%mode%"=="continue" (
		cd build
		set /p seed= < testModuleConfig	
		echo #############continue last mission,current seed:!seed!		
		goto :EOF
	) 
	if "%mode%"=="new" (
		set seed=-1
		echo Create build folder
		rd /s/q build_last 1>nul 2>nul
		rename build build_last 1>nul 2>nul
		call :recreateFolder build & cd build		
		call %~dp0/copyGstatics
		call %~dp0/copystatics
		set t={"octave":"%~dp0Octave\Octave-3.8.2\bin\octave.exe"}		
		echo !t:\=\\! > global.json
		goto :EOF
	) 
	echo UKNOWN OPTION
	goto :ASKMODE
:JOINPATH
	set Path1=%~1
	set Path2=%~2
	if {%Path1:~-1,1%}=={\} (set return=%Path1%%Path2%) else (set Result=%Path1%\%Path2%)
	goto :eof
:GOODEND
	start /i !build!\nw.exe 
:END
	echo END
	goto :HEAD