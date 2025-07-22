#!/bin/bash

cd /code/opencv/build && make -j$(cat /proc/cpuinfo | grep -c 'processor')
make install
cpack -G DEB

cp /code/opencv/build/*.deb /output

make uninstall