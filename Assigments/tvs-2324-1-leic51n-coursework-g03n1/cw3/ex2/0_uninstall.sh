#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

systemctl stop tvsapp.service
sleep 2
systemctl status tvsapp.service --no-pager

echo "------------------------"

systemctl stop tvsapp.socket
sleep 2
systemctl status tvsapp.socket --no-pager

echo "------------------------"

SYSTEM_MD_DIR=/etc/systemd/system
rm -f $SYSTEM_MD_DIR/tvsapp.service
rm -f $SYSTEM_MD_DIR/tvsapp.socket
# rm -i /opt/isel/tvs/tvsctld/bin/*

systemctl daemon-reload

