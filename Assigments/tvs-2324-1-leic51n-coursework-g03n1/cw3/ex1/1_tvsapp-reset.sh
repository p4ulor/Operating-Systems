#!/bin/bash

SITES_AVAILABLE="/etc/nginx/sites-available/tvsapp"
SCALE=${1:-1} # scale = 1 if param not present
BASE=${2:-39000} # base = 39000 if param not present

# check if NGINX configuration file exists
if [ ! -f "$SITES_AVAILABLE" ]; then
    echo "SITES_AVAILABLE file not found: $SITES_AVAILABLE"
    exit 1
fi

# Create the new server entries
NEW_SERVERS=""
for (( i=0; i<SCALE; i++ )); do
    PORT=$((BASE + i))
    NEW_SERVERS+="server 127.0.0.1:$PORT;\n"
done

echo "Servers added = $NEW_SERVERS"

# Remove the existing upstream block
sed -i '/upstream tvsapp {/,/}/d' "$SITES_AVAILABLE"
# Append the new server entries to the upstream block
echo -e "upstream tvsapp {" >> "$SITES_AVAILABLE"
echo -e "$NEW_SERVERS" >> "$SITES_AVAILABLE"
echo -e "}" >> "$SITES_AVAILABLE"

echo "NGINX configuration updated with $SCALE instances starting at port $BASE."

