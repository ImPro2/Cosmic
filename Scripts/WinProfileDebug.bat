@echo on

cd ..

start Tools/Optick.exe "bin/Debug-windows-x86_64/Sandbox/Sandbox.exe"
timeout /T 4
start cmd /c start bin/Debug-windows-x86_64/Sandbox/Sandbox.exe