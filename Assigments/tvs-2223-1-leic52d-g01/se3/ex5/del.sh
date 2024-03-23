#!/bin/bash

filename=/etc/nginx/sites-available/tvsapp
servicename=tvsapp@$1

if grep -q $1 $filename; then
    sudo sed -i '/'$1'/d' $filename
    sudo systemctl stop $servicename
    sudo systemctl disable $servicename
    PID=$(sudo cat /var/run/nginx.pid)
    sudo kill -HUP $PID
    sudo systemctl reset-failed
else
    echo There Port $1 was not configured
fi
