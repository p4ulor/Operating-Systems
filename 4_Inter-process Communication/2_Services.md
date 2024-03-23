
- sudo systemctrl status elasticsearch
- cat /lib/systemd/system/multi-user.target
- grep ":[0-9]\+;" /etc/nginx/sites-available/tvsapp | cut -d : -f 2 | tr -d \;
- grep ":[0-9]\+;" /etc/nginx/sites-available/tvsapp | cut -d : -f 2 | tr -d \; | tail -n 1
- grep ":[0-9]\+;" /etc/nginx/sites-available/tvsapp | cut -d : -f 2 | tr -d \; | head -n -2