#!/bin/bash

filename=/etc/nginx/sites-available/tvsapp
link=/etc/nginx/sites-enabled/tvsapp

find='server 127.0.0.1:'
while read p; do
    if grep -q "$find" <<< "$p"; then
        # takes the prefix of the line to isolate the port number
        aux=${p#*:}
        # takes the semicolon that comes after the number
        port=${aux%;*}
        servicename=tvsapp@$port
        # code repeat of the delete bash
        sudo systemctl stop $servicename
        sudo systemctl disable $servicename
        sudo systemctl reset-failed
    fi
done < $filename

if [ -f $link ]; then
    sudo rm $link
    PID=$(sudo cat /var/run/nginx.pid)
    sudo kill -HUP $PID
    echo "tvsapp was shutdown"
else
    echo "tvsapp was already down"
fi


