## 1_tvsapp-reset.sh
- sudo bash 1_tvsapp-reset.sh 2 4000

Output:
```
server 127.0.0.1:4000;\nserver 127.0.0.1:4001;\n
NGINX configuration updated with 2 instances starting at port 4000.
```
- nginx /etc/nginx/sites-available
```c

server {
    listen 12021 default_server;
    server_name _;

    location / {
        proxy_pass http://tvsapp/;
        proxy_set_header Host $host;
    }
}

upstream tvsapp {
server 127.0.0.1:4000;
server 127.0.0.1:4001;

}
```

## 5_tvsapp-start.sh
- sudo bash 5_tvsapp-start.sh 

Output:
```
Elasticsearch is already running
ports found in sites available: 4000 4001
Started server with port 4000
Started server with port 4001
tvsapp was found in sites-enabled, will delete it!
nginx reloaded
```
## 2_tvsapp-inc.sh
- sudo bash 2_tvsapp-inc.sh 2

Output:
```
Starting tvsapp services from port 4002 onwards...
started on 4002
started on 4003
```

## 3_tvsapp-dec.sh
- sudo bash 3_tvsapp-dec.sh 2

Output:
```
Ports used by tvsapp.service instances: 4000
4001
4002
4003
Number of tvsapp.service instances running: 4
Stopped on port 4003
Stopped on port 4002
Remaining ports now being used: 4000
4001
```

## 6_tvsapp-status.sh
- sudo bash 6_tvsapp-status.sh

Output:
```
-  nginx.service loaded active running A high performance web server and a reverse proxy server
-  tvsapp@4000.service loaded active running TVS Web Application
 tvsapp@4001.service loaded active running TVS Web Application
-  elasticsearch.service loaded active running Elasticsearch
```

## 4_tvsapp-stop.sh
- sudo bash 4_tvsapp-stop.sh

Output:
```
Stopped server with port 4001
Stopped server with port 4000
nginx reloaded
elastic search will remain running
```

