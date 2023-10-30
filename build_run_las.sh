#!/bin/bash

echo -e "Build and run las system"
cd build
cmake .. && make

cd .. && ./build/Las

