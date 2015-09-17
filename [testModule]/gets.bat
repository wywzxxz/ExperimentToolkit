@echo off
:HEAD
set /p a=
if "%a%"=="patch"(
	goto :HEAD
)
if "%a%"=="fullpatch"(
	goto :HEAD
)
if "%a%"=="break"(
	cd !build!\tmp
	echo aaa >gets_tmp.txt
	echo                                            //#cancling the assignment#\\	
	goto :HEAD
)
echo ORDER: patch\fullpatch\break