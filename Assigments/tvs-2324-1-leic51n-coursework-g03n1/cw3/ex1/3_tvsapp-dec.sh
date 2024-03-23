#!/bin/bash

SITES_ENABLED=/etc/nginx/sites-enabled/tvsapp

if [ "$(id -u)" -ne 0 ]; then
   echo "Only an admin can do that."
   exit 1
fi

DELTA=${1:-1}

# Get the ports of running tvsapp.service instances
ports=$(systemctl list-units | grep 'tvsapp' | grep -oE '[0-9]+')
echo "Ports used by tvsapp.service instances: $ports"

# Convert the ports to an array for iteration. -t removes new line from the end of each line read. 
readarray -t ports_array <<< "$ports"

# Calculate the number of instances
service_count=$(echo -e "$ports" | wc -l) #-e flag in echo enables the interpretation of backslash escapes, including \n for newlines
if [ "$service_count" -eq 0 ]; then
    echo "No instances of tvsapp.service are running"
    exit 1
fi
echo "Number of tvsapp.service instances running: $service_count"

if [ "${#ports_array[@]}" -eq 1 ]; then
    echo "Atleast 1 Node.js instace should be running. Exiting"
    exit 1
fi

# Stop servers in decrementing order of ports
for (( i = service_count - 1; i >= 0 && DELTA > 0; i-- )); do
    PORT="${ports_array[i]}"
    systemctl stop "tvsapp@$PORT.service"
    echo "Stopped on port $PORT"
    let "DELTA-=1"
done

# Print final status
ports=$(systemctl list-units | grep 'tvsapp' | grep -oE '[0-9]+')
echo "Remaining ports now being used: $ports"