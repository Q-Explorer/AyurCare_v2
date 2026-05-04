#Instructions

1. Download the zip file: AyurCare_C++_with_GUI.zip

2. Unzip the file and open the folder in VS code

3. In VS code press:

Ctrl + ` (the key which is below esc)

This will open the terminal

4. Copy and paste the following command and hit enter, then wait for a while for a new line (your project path in your local storage) to appear:

g++ AyurCare_GUI.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp -I. -Iimgui -Iimgui/backends -L. -lglfw3 -lopengl32 -lgdi32 -o AyurCare.exe

5. After the new line appears, enter the following command to run the executable formed:
 
./AyurCare.exe
