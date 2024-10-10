#!/usr/bin/bash

if [ $# == 0 ]; then
echo Provide the service to be removed
exit 1
fi

#  Verify SU
if [ ! $USER == root ]; then
echo Run as root
exit 1
fi

# Copying scripts
_DST_SBIN="/usr/local/sbin"


#SERVICE=mount_tmpfs.service
SERVICE=$1



systemctl stop $SERVICE
systemctl disable $SERVICE
rm /etc/systemd/system/$SERVICE
rm /etc/systemd/system/$SERVICE #and symlinks that might be related
rm /usr/lib/systemd/system/$SERVICE
rm /usr/lib/systemd/system/$SERVICE # and symlinks that might be related
systemctl daemon-reload
systemctl reset-failed

