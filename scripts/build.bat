@echo off
setlocal

for %%I in ("%~dp0..") do set "ROOT_DIR=%%~fI"

set "BUILD_DIR=%ROOT_DIR%\build"
set "RUNTIME_DIR=%ROOT_DIR%\runtime"
set "SOURCE=%BUILD_DIR%\bin\whispy.exe"
set "DEST=%RUNTIME_DIR%\whispy.exe"

echo.
echo [Whispy] Configuring CMake...
cmake -S "%ROOT_DIR%" -B "%BUILD_DIR%"

if errorlevel 1 (
    echo.
    echo [Whispy] CMake configuration failed.
    pause
    exit /b 1
)

echo.
echo [Whispy] Building Debug...
cmake --build "%BUILD_DIR%" --config Debug

if errorlevel 1 (
    echo.
    echo [Whispy] Build failed.
    pause
    exit /b 1
)

if not exist "%SOURCE%" (
    echo.
    echo [Whispy] Built executable was not found:
    echo %SOURCE%
    pause
    exit /b 1
)

if not exist "%RUNTIME_DIR%" (
    mkdir "%RUNTIME_DIR%"
)

echo.
echo [Whispy] Copying executable...
copy /Y "%SOURCE%" "%DEST%" >nul

if errorlevel 1 (
    echo.
    echo [Whispy] Failed to copy executable.
    pause
    exit /b 1
)

echo.
echo ========================================
echo [Whispy] Build successful!
echo [Whispy] Runtime: %DEST%
echo ========================================
echo.

pause