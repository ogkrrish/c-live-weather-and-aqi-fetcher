@echo off
echo.
echo ==========================================
echo   Compiling Weather App
echo ==========================================
echo.
gcc main.c -o weather.exe
if %errorlevel% neq 0 (
    echo.
    echo ==========================================
    echo   Compilation FAILED!
    echo ==========================================
    echo.
    pause
    exit /b %errorlevel%
)
echo ==========================================
echo   Compilation successful!
echo ==========================================
echo.
echo Starting weather app...
echo.
weather.exe
