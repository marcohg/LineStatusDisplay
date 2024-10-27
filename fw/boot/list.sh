#!/usr/bin/bash

_DST_SYSTEMD="/etc/systemd/system"
if [ $USER == root ]; then
  echo Run this script as root
  cd ..
  return 1 2>/dev/null # ignore 'return' error 
  exit 1  # finish when sourcing
fi
echo
echo "Entering $USER folder... "
cd $USER
echo 
_SERVICES="$(ls *.service)"
echo 
echo List Installed services
for i in $_SERVICES; do
  sudo systemctl status $i
done

echo Exiting $USER ...
cd ..