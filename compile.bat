cd MinGW\bin
mingw32-g++.exe -O3 -std=c++98  -c ..\..\main.cpp -o ..\..\obj\main.o
mingw32-g++.exe  -o ..\..\BUILD\unnamed-rpg.exe ..\..\obj\main.o  -s  "..\lib\liballegro_monolith.dll.a"
cd ..\..