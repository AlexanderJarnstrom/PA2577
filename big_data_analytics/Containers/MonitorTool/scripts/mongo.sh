#!/bin/sh

set -e

curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r4.1.4/mongo-cxx-driver-r4.1.4.tar.gz
tar -xzf mongo-cxx-driver-r4.1.4.tar.gz
cd /usr/src/mongo-cxx-driver-r4.1.4/build
cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
cmake --build . -j 4
cmake --build . --target install
