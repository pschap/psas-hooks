#!/bin/bash

cd build
make clean
make all
cd ..

curl --ftp-method nocwd -T build/psas-hooks.suprx ftp://192.168.1.75:1337/ur0:/tai/
echo launch PCSA00069 | nc 192.168.1.75 1338
