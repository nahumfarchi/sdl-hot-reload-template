@echo off

:: (1) Run this to open the exe in VS (make sure that the .pdb exists and is found. You can right click the solution and select "Debug->Step into new instance" to see that everything works as expected).
::devenv build\win32_handmade.exe
::devenv build\debug\main_release.exe
devenv build\hot_reload\main_hot_reload.exe

:: (2) Once the solution is saved, open it using:
::start devenv misc\win32_handmade.sln