#!/bin/bash

cd build
make clean
make all
cd ..

curl --ftp-method nocwd -T build/psas-hooks.suprx ftp://IP_TO_VITA:1337/ur0:/tai/
echo launch PCSA00069 | nc IP_TO_VITA 1338
