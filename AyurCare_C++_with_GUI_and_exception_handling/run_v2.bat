@echo off
cd /d "%~dp0"

echo ==============================
echo AyurCare Build Started
echo ==============================
echo.

echo [1/3] Checking required files...

if not exist AyurCare_GUI.cpp (
    echo ERROR: AyurCare_GUI.cpp not found.
    pause
    exit /b
)

if not exist imgui\imgui.cpp (
    echo ERROR: ImGui files not found.
    pause
    exit /b
)

if not exist imgui\backends\imgui_impl_glfw.cpp (
    echo ERROR: ImGui GLFW backend not found.
    pause
    exit /b
)

if not exist libglfw3.a (
    echo ERROR: libglfw3.a not found.
    pause
    exit /b
)

if not exist glfw3.dll (
    echo ERROR: glfw3.dll not found.
    pause
    exit /b
)

echo All required files found.
echo.

echo [2/3] Compiling project...

g++ AyurCare_GUI.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp -I. -Iimgui -Iimgui/backends -L. -lglfw3 -lopengl32 -lgdi32 -o AyurCare.exe

if errorlevel 1 (
    echo.
    echo BUILD FAILED.
    echo Please check the error messages above.
    pause
    exit /b
)

echo Build successful.
echo.

echo [3/3] Starting AyurCare...
echo.

AyurCare.exe

echo.
echo AyurCare closed.
pause