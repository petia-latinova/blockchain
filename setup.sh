#!/bin/bash
cd lib/secp256k1
./autogen.sh
./configure
make
sudo make install
cd -