mkdir build
cd build
set PATH=C:\msys64\mingw64\bin;%PATH%
cmake -G "MinGW Makefiles" ../ 
cmake --build .
ctest .