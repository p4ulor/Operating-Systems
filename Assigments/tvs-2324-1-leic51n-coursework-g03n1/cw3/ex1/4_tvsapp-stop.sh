#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

# Part 1 - Stop all web app instances

SITES_AVAILABLE=/etc/nginx/sites-available/tvsapp
SITES_ENABLED=/etc/nginx/sites-enabled/tvsapp

# Get the ports of running tvsapp.service instances
ports=$(systemctl list-units | grep 'tvsapp' | grep -oE '[0-9]+')

# Convert the ports to an array for iteration
readarray -t ports_array <<< "$ports"

# Calculate the number of instances
service_count=$(echo -e "$ports" | wc -l) #-e flag in echo enables the interpretation of backslash escapes, including \n for newlines
if [ "$service_count" -eq 0 ]; then
    echo "No instances of tvsapp.service are running"
    exit 1
fi

# Stop servers in decrementing order of ports
for (( i = service_count - 1; i >= 0; i-- )); do
    PORT="${ports_array[i]}"
    systemctl stop "tvsapp@$PORT.service"
    echo "Stopped server with port $PORT"
done

# Part 2 - Remove servetvsapp link from sites-enabled

if [ -f $SITES_ENABLED ]; then
    sudo rm $SITES_ENABLED
    sudo systemctl reload nginx
    echo nginx reloaded
else
    echo "tvsapp already not in sites enabled"
fi

# Part 3 - Disable elastic search if prompted
if [[ "$1" == "-db" ]]; then
    systemctl stop elasticsearch.service
    echo elastic search also stopped
else
    echo elastic search will remain running
fi
