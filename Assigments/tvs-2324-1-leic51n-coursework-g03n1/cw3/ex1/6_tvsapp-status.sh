#!/bin/bash

# sed 's/ \+/ /g' is being used to remove extra spaces. \+ looks for one or more spaces. the replacement is s/ = 1 space. /g = any (global) pattern

all_services=$(systemctl list-units)

nginx_status=$(echo "$all_services" | grep nginx | sed 's/ \+/ /g')
echo "- $nginx_status"

systemctl status nginx --no-pager

tvsapps_status=$(echo "$all_services" | grep "TVS Web Application" | sed 's/ \+/ /g')
echo "- $tvsapps_status" 

elasticsearch_status=$(echo "$all_services" | grep elasticsearch | sed 's/ \+/ /g')
echo "- $elasticsearch_status"
