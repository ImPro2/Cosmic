@echo off

cd ..

cd Dependencies\Optick
call ..\..\Tools\premake5.exe vs2022

cd ..\..\

call Tools\premake5.exe vs2022

pause