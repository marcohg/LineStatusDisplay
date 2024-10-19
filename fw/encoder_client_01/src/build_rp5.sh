#! /bin/bash
# Simple CLI to compile main.cpp
_DST="/usr/local/sbin"
_EXE=encoder_client

echo 
echo "Test $DST folder"
if [ ! -d $_DST ]; 
  then mkdir $_DST
fi
echo "Compile encoder files" 
g++ -O0 -Wall -I/usr/local/include/modbus -v main.cpp  c_modbus.cpp read_config.cpp -L/usr/local/lib  -lmodbus  -o $_DST/$_EXE

echo
echo "Target is $_DST/$_EXE"

