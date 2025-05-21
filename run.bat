@echo off
echo Running on Windows...
cmake -B build
cmake --build build

build\bin\Main.exe
pause