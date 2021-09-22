rem Use this batch file to build CrunchMath for Visual Studio

rem Relpace this path with your cmake installation path:- location of cmake on your PC
set PATH="C:/cmake/bin";%PATH%

cmake .
rem To force 32bit architecture builds
rem cmake . -A "Win32"

PAUSE