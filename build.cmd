mkdir build
cd build
set PATH=C:\msys64\mingw64\bin;%PATH%
cmake -G "MinGW Makefiles" ../ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build .
ctest .