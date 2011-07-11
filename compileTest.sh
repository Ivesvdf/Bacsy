#!/usr/bin/env bash
mkdir build >/dev/null 2>/dev/null
cd build
cmake .. && make && ./src/test/test
