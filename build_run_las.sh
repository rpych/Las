#!/bin/bash

make clean
echo -e "Build and run las system"
cd build
cmake .. && make

echo -e "Running googletests"

ctest -R LasTestParametrized -V

#cd ../ && ./build/las diff
