#!/bin/bash

SITES_ENABLED=/etc/nginx/sites-enabled/tvsapp

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

DELTA=${1:-1}
BASE=29900

# Get the ports of running tvsapp.service instances
ports=$(systemctl list-units | grep 'tvsapp' | grep -oE '[0-9]+')

# Find the highest port in use among tvsapp.service instances
highest_port=$(echo "$ports" | sort -rn | head -n 1)
if [ -n "$highest_port" ]; then
    BASE=$((highest_port + 1))
fi

echo "Starting tvsapp services from port $BASE onwards..."

# Create the new server entries
for (( i=0; i<DELTA; i++ )); do
    PORT=$((BASE + i))
    systemctl start tvsapp@$PORT.service
    echo started on $PORT
done
