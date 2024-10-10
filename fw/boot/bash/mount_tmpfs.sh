#!/usr/bin/bash
# mount_tmpfs.sh - run script as root
mount -t tmpfs -o size=20m tmpfs /mnt/tmp
chmod -R 777 /mnt/tmp
