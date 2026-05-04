@echo off
cd /d "%~dp0"

echo Checking files...
echo.

if not exist AyurCare_GUI.cpp (
    echo ERROR: AyurCare_GUI.cpp not found
    pause
    exit /b
)

if not exist libglfw3.a (
    echo ERROR: libglfw3.a not found
    pause
    exit /b
)

if not exist glfw3.dll (
    echo ERROR: glfw3.dll not found
    pause
    exit /b
)

if not exist imgui\imgui.cpp (
    echo ERROR: imgui files not found
    pause
    exit /b
)

echo Building...
echo.

g++ AyurCare_GUI.cpp ^
imgui/imgui.cpp ^
imgui/imgui_draw.cpp ^
imgui/imgui_tables.cpp ^
imgui/imgui_widgets.cpp ^
imgui/backends/imgui_impl_glfw.cpp ^
imgui/backends/imgui_impl_opengl3.cpp ^
-I. ^
-Iimgui ^
-Iimgui/backends ^
libglfw3.a ^
-lopengl32 ^
-lgdi32 ^
-luser32 ^
-lshell32 ^
-lwinmm ^
-o AyurCare.exe

if errorlevel 1 (
    echo.
    echo BUILD FAILED
    pause
    exit /b
)

echo.
echo BUILD SUCCESSFUL
echo Starting program...
echo.

AyurCare.exe

echo.
echo Program closed.
echo Exit code: %errorlevel%
pause