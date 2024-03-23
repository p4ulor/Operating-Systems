#!/bin/bash

filename=/etc/nginx/sites-available/tvsapp

if grep -q $1 $filename; then
    echo Port $1 was already added
else
    sudo sed -i $'2i\ \tserver 127.0.0.1:'$1';' $filename

    sudo systemctl start tvsapp@$1
    PID=$(sudo cat /var/run/nginx.pid)
    sudo kill -HUP $PID
fi

