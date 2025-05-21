#!/bin/bash
set -e

cmake -B build
cmake --build build

./build/bin/Main

