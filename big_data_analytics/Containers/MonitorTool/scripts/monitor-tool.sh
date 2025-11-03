#!/bin/sh

set -e

rm -rf /usr/src/MonitorTool/build
mkdir -p /usr/src/MonitorTool/build
cd /usr/src/MonitorTool/build
cmake ..
make
ldd /usr/src/MonitorTool/build/monitor-tool
