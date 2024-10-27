#!/usr/bin/bash

remove_service() {
  systemctl stop $1
  systemctl disable $1
  rm /etc/systemd/system/$1 2>/dev/null
  rm /etc/systemd/system/$1 2>/dev/null #and symlinks that might be related
  rm /usr/lib/systemd/system/$1 2>/dev/null
  rm /usr/lib/systemd/system/$1 2>/dev/null # and symlinks that might be related
  systemctl daemon-reload
  systemctl reset-failed
  sleep 1
}

# -------------------
# script
# -------------------
if [ $# == 0 ]; then
  echo Provide the name.service or the folder name
  return 1 2>/dev/null || exit 1  # exit on execute/source
fi

#  Run as sudo (avoid interactive systemctl)
if [ ! $USER == root ]; then
  echo Run as root 
  return 1 2>/dev/null || exit 1  # exit on execute/source
fi

# Get services: user defined services in folder or remove a single service
ORIG_USER=$(logname)
if [ $1 == $ORIG_USER ]; then
  echo Entering $ORIG_USER folder ...
  cd $ORIG_USER
  _SERVICES="$(ls *.service)"
  echo Leaving $ORIG_USER ..
  cd ..
else
  _SERVICES=$1
fi

echo 
echo Removing $_SERVICES ...
for i in $_SERVICES; do
  remove_service $i
done

exit 0
  
  
  






