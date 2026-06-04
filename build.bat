@echo off
setlocal EnableExtensions

set "UE=C:\Program Files\Epic Games\UE_5.7"
set "UPROJECT=C:\Users\aleja\Documents\Unreal Projects\QAA\QAA.uproject"
set "TARGET=QAAEditor"
set "PLATFORM=Win64"
set "CONFIG=Development"

REM tiempo inicio (ms)
for /f %%i in ('powershell -NoProfile -Command "[DateTimeOffset]::Now.ToUnixTimeMilliseconds()"') do set "START_MS=%%i"

REM build
call "%UE%\Engine\Build\BatchFiles\Build.bat" %TARGET% %PLATFORM% %CONFIG% "-Project=%UPROJECT%" -WaitMutex
set "BUILD_RC=%ERRORLEVEL%"

REM tiempo fin (ms)
for /f %%i in ('powershell -NoProfile -Command "[DateTimeOffset]::Now.ToUnixTimeMilliseconds()"') do set "END_MS=%%i"

REM duracion seg con 3 decimales y coma (08,383)
for /f %%i in ('powershell -NoProfile -Command ^
  "$s=%START_MS%; $e=%END_MS%; $sec=($e-$s)/1000.0; '{0:00,000}' -f $sec"') do set "DURATION=%%i"

REM hora fin HH:mm
for /f %%i in ('powershell -NoProfile -Command "(Get-Date).ToString('HH:mm')"') do set "END_HHMM=%%i"

REM resumen estilo VS + abrir editor si OK
echo.
if %BUILD_RC% NEQ 0 (
  echo ========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
  echo ========== Build completed at %END_HHMM% and took %DURATION% seconds ==========
  echo.
  pause
  exit /b %BUILD_RC%
) else (
  echo ========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
  echo ========== Build completed at %END_HHMM% and took %DURATION% seconds ==========
  echo.
)

exit /b 0
