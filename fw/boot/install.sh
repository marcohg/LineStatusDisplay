#!/usr/bin/bash

_DST_SYSTEMD="/etc/systemd/system"
ORIG_USER=$(logname)

echo
echo "Entering $ORIG_USER folder... "
cd $ORIG_USER
_SERVICES="$(ls *.service)"

echo "copy $_SERVICES to $_DST_SYSTEMD and 'systemctl enable' it"
for i in $_SERVICES; do
  sudo cp $i $_DST_SYSTEMD
  sudo systemctl enable $i
done

echo Exiting $ORIG_USER ...
cd ..
echo Installation complete - reboot target to verify...
