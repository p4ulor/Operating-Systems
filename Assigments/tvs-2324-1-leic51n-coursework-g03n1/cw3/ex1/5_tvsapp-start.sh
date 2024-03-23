#!/bin/bash

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

# Part 1 - Elastic search
if systemctl is-active --quiet elasticsearch.service; then
    echo "Elasticsearch is already running"
else
    sudo systemctl start elasticsearch.service
    echo "Elasticsearch has been started"
fi

# Part 2 - Start services in "/etc/nginx/sites-available"
SITES_AVAILABLE=/etc/nginx/sites-available/tvsapp
SITES_ENABLED=/etc/nginx/sites-enabled/tvsapp

# check if NGINX configuration file exists
if [ ! -f "$SITES_AVAILABLE" ]; then
    echo "SITES_AVAILABLE file not found: $SITES_AVAILABLE"
    exit 1
fi

# Gets ports in SITES_AVAILABLE
ports=$(grep -oE '#*:[0-9]+' "$SITES_AVAILABLE" | tr -d ':')
echo ports found in sites available: $ports
readarray -t ports_array <<< "$ports"

if [ "${#ports_array[@]}" -eq 0 ]; then
    echo "No tvsapp block found with server and ports in $SITES_AVAILABLE"
    exit 1
fi

# Start servers found in the SITES_AVAILABLE
for (( i = 0; i < "${#ports_array[@]}"; i++ )); do
    PORT="${ports_array[i]}"

    if systemctl is-active --quiet "tvsapp@$PORT.service"; then
    echo "tvsapp@$PORT.service is already running, skipping"
    else
        systemctl start "tvsapp@$PORT.service"
        echo "Started server with port $PORT"
    fi
done

# Part 3 - Deleting existing sites enabled
if [ -f $SITES_ENABLED ]; then
    echo "tvsapp was found in sites-enabled, will delete it!"
    rm $SITES_ENABLED
fi

# make folder-link in enabled to available and reload nginx
sudo ln -s $SITES_AVAILABLE $SITES_ENABLED
systemctl reload nginx
echo "nginx reloaded"

