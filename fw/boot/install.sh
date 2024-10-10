#!/usr/bin/bash

#  Verify SU
if [ ! $USER == root ]; then
echo Run as root
exit 1
fi

# Copying scripts
_DST_SBIN="/usr/local/sbin"
_DST_SYSTEMD="/etc/systemd/system"
echo
echo "Entering 'bash/'..."
cd bash
_SHS=$(ls *.sh)

echo "Copying "$_SHS "scripts to "$_DST_SBIN
cp $(ls *.sh) $_DST_SBIN

echo Chmod to allow all users to r/w and eXecute
for i in $_SHS; do
chmod 777 $_DST_SBIN/$i
done
echo exiting  'bash/'..
cd ..

echo
echo "-- systemd services --"
echo "Entering 'systemd/'..."
cd systemd
_SERVICES="$(ls *.service)"
echo  "Do manually the following: "
echo "1. remove desired service, if already installed"
echo "2. copy [sudo] $_SERVICES to $_DST_SYSTEMD"
echo "3. Use systemctl enable [service] and reboot"
echo "Exiting 'systemd/'..."


echo
echo "Install complete"
exit 0

