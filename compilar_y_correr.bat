@echo off
cd /d "C:\c++\OpenGL-VEnvironment"
echo [1/3] Compilando proyecto...
"C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build_win --config Release

if %errorlevel% equ 0 (
    echo [2/3] Copiando shaders...
    if not exist "build_win\Release\src\shaders" mkdir "build_win\Release\src\shaders"
    xcopy /y /s "src\shaders\*" "build_win\Release\src\shaders\"
    
    echo [3/3] Lanzando aplicacion...
    start "" ".\build_win\Release\app.exe"
) else (
    echo [ERROR] La compilacion fallo.
    pause
)
