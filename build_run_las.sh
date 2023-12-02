#!/bin/bash

make clean
echo -e "Build and run las system"
cd build
cmake .. && make

cd ../test/build
cmake .. && make

echo -e "Running googletests"

#ctest --target las_test

cd ../.. && ./build/las diff --include

