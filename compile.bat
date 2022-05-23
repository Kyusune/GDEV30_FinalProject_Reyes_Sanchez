@echo off
set include_folder="C:\Users\Gio Reyes\Desktop\School\3rd Year\2ndSem\GDev30\OpenGL\Include"
set libraries_folder="C:\Users\Gio Reyes\Desktop\School\3rd Year\2ndSem\GDev30\OpenGL\Libraries"

@echo on
g++ *.cpp glad.c -o out -I %include_folder% -L %libraries_folder% -lglfw3 -lopengl32 -mwindows
pause