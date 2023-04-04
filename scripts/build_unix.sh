#!/bin/bash

build_unix()
{
	echo "BUILD PROJECT START" 
	cd ..
	mkdir build
	cd build
	cmake --no-warn-unused-cli \
		-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
		-DCMAKE_BUILD_TYPE:STRING=Debug \
		-DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc-12 \
		-S/home/genater/repositories/QuickChat \
		-B/home/genater/repositories/QuickChat/build \
		-G "Unix Makefiles"
	cmake --build .
	echo "BUILD PROJECT END" 
}

build_unix