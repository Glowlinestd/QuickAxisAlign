@echo off
setlocal EnableExtensions

set "UE=C:\Program Files\Epic Games\UE_5.8"
set "UPROJECT=C:\Users\aleja\Documents\Unreal Projects\QAA\QAA.uproject"
set "TARGET=QAAEditor"
set "PLATFORM=Win64"
set "CONFIG=Development"


REM lanzar Unreal Editor con el proyecto
start "" "%UE%\Engine\Binaries\Win64\UnrealEditor.exe" "%UPROJECT%"

exit /b 0
