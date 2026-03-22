@echo off
echo =======================================
echo 1. PREPARANDO ENTORNO (CMAKE)
echo =======================================
:: Intenta usar el CMake interno de Visual Studio (Para ti), o el global del sistema (Para tus amigos)
"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -B build_win -S . 2>NUL || cmake -B build_win -S .

echo.
echo =======================================
echo 2. COMPILANDO PROYECTO (C++)
echo =======================================
"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_win --config Release 2>NUL || cmake --build build_win --config Release

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Hubo un problema compilando el codigo. Revisa los errores arriba.
    pause
    exit /b %errorlevel%
)

echo.
echo =======================================
echo 3. LANZANDO APLICACION
echo =======================================
start .\build_win\Release\app.exe
