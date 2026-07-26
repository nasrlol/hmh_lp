@echo off
setlocal

set BUILD_DIR=build
set SOURCE_DIR=src

set GLFW_DIR=W:\include\glfw
set VULKAN_DIR=C:\VulkanSDK\1.4.350.0

set INCLUDES=-I%SOURCE_DIR%\code -I%SOURCE_DIR% -I%SOURCE_DIR%\base -I%GLFW_DIR%\include -I%SOURCE_DIR%\render
set CFLAGS=-FC -GR- -EHa- -nologo -Zi -FS /analyze /W3 /WX -MD

set LIBS=user32.lib gdi32.lib kernel32.lib shell32.lib opengl32.lib vulkan-1.lib glfw3.lib
set LN=/link /LIBPATH:%GLFW_DIR%\lib-vc2015  /LIBPATH:%VULKAN_DIR%\Lib

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

echo Building %BUILD_DIR%\program.exe...

cl %INCLUDES% %CFLAGS%  %SOURCE_DIR%\code\program.cpp  /Fe%BUILD_DIR%\program.exe %LN% %LIBS%