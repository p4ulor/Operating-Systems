#!/bin/bash

#disables the --more option built into the bash terminal
set page-completions Off 

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

SERVER_LOCATION=/opt/isel/tvs/tvsctld/bin
currentPath=$(pwd)
ex1Path="${currentPath%/*}/ex1"
# --preserve AKA -p to not do a sudo file copy and have it's owners be root
cp --preserve $ex1Path/* $SERVER_LOCATION

SYSTEM_MD_DIR=/etc/systemd/system
cp tvsapp.service $SYSTEM_MD_DIR 
cp tvsapp.socket $SYSTEM_MD_DIR

# reloads systemmd files. If you change a service file in /etc/systemd/system/ , daemon-reload will reload these files
systemctl daemon-reload
sleep 2

systemctl start tvsapp.socket
sleep 2
systemctl status tvsapp.socket --no-pager

echo "------------------------"

systemctl start tvsapp.service
sleep 2
systemctl status tvsapp.service --no-pager

echo "------------------------"

# sudo journalctl -u tvsapp.socket
