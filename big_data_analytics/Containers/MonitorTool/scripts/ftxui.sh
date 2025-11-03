#!/bin/sh

set -e

git clone https://github.com/ArthurSonzogni/FTXUI.git
mkdir FTXUI/build
cd /usr/src/FTXUI/build
cmake ..
make install -j 4
