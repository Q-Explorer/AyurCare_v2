Instructions

1. Download the raw zip file: AyurCare_C++_with_GUI.zip
<img width="1439" height="452" alt="image" src="https://github.com/user-attachments/assets/ca42e7f5-86c3-46f2-b066-7e7f09a3f328" />

2. Unzip the file and open the folder in VS code

3. In VS code press:

     Ctrl + ` (the key which is below esc)

     This will open the terminal

4. Copy and paste the following command and hit enter:

g++ AyurCare_GUI.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp -I. -Iimgui -Iimgui/backends -L. -lglfw3 -lopengl32 -lgdi32 -o AyurCare.exe

5. Wait for sometime for a new line (your project path in local storage) to appear.

6. After the new line appears, enter the following command to run the executable formed:
 
./AyurCare.exe
