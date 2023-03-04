@echo off

set TEMP_DIR=tmp
set TEMP_SCRIPT=sgl_launch_game
set LIBS_DIR=modules
set LIBS_IO_DIR=data\io

:: Set current dir of this script
cd /d %~dp0

:: Have dlls in subfolder to avoid messy root dir
:: Also include libs in data/io for IO libs that have further dependencies
set PATH=%LIBS_DIR%;%LIBS_IO_DIR%;%PATH%

:: Run loader
"%cd%\sgl.exe" data/settings.lua

:: Execute temp script which starts the selected game
if exist %TEMP_DIR%\%TEMP_SCRIPT% (
	call %TEMP_DIR%\%TEMP_SCRIPT%

	:: Start game loader again
	%cd%\sgl.bat
)