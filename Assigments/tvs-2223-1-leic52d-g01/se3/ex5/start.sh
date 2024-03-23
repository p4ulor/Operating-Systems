#!/bin/bash

filename=/etc/nginx/sites-available/tvsapp
find='server 127.0.0.1:'
link=/etc/nginx/sites-enabled/tvsapp

while read p; do
    if grep -q "$find" <<< "$p"; then
        # takes the prefix of the line to isolate the port number
        aux=${p#*:}
        # takes the semicolon that comes after the number
        port=${aux%;*}
        servicename=tvsapp@$port
        # code repeat of the add bash
        sudo systemctl start $servicename

    fi
done < $filename

if [ ! -f $link ]; then
    sudo ln -s $filename $link
    PID=$(sudo cat /var/run/nginx.pid)
    sudo kill -HUP $PID
    echo "tvsapp was initiated"
else
    echo "tvsapp was already up"
fi
