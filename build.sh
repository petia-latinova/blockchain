#!/bin/bash
mkdir -p build
g++ -o build/node -lsecp256k1 src/node.cpp