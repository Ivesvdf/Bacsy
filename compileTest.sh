#!/usr/bin/env bash
mkdir build >/dev/null 2>/dev/null
cd build
cmake -DCMAKE_FIND_ROOT_PATH=$(cd ..; cd external; pwd) .. && make && ./src/test/test
