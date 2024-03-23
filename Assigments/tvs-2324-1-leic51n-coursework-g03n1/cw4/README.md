# Ex1 - Dockerfile
Inside [tvsapp](./tvsapp/)
## Build image
- docker build -t tvs-ex1 .
## Run container
- docker run -d -p 4001:4003 --name tvs-ex1-c1 tvs-ex1
- docker run -d -p 4002:4003 --name tvs-ex1-c2 tvs-ex1
- docker run -d -p 4003:4003 --name tvs-ex1-c3 tvs-ex1
- docker run -d -p 4004:4003 --name tvs-ex1-c4 tvs-ex1

## Exercise notes:
### 1. Regarding the `Produce the tvsapp image from this Dockerfile and use docker run to get four containers in execution, making the application available in ports 4001, 4002, 4003, and 4004 of the host system"`
We observed that when we did the copy of all the files in src/ the number of layers was 7, and after individually copying the `tvsapp.js` to the working dir, the number of layers increased to 8

### 2. Regarding the `Be ready to explain the values observed for HOST and PORT in the four instances.`
We are ready to explain :D

- Host -> refers to the machine where the Docker containers are running. My machine.
- Port -> Refers to the port number on the host system that is mapped to the port inside the Docker container. And for each container, the ports 4001 to 4004 of the host are mapped to the port 4003 inside the container. The application running inside the container on port 4003 is accessible from the host system via all of these ports set on the `docker run` command

## General notes:
- `-d` runs the container in detached mode, meaning it's run in the background, so it's not binded & dependent on an opened bash console
- RUN commands are executed during the Docker image build process, not when the container is running
- To interact with the container's file system: `docker run -it tvs-ex1 /bin/sh`
- docker images -a //list images
- docker ps //list running containers
- docker inspect «container-name»
- curl http://172.17.0.2:«PORT» (the IPAddress is the container's IP address, this can be consulted in the array w/ the object at "NetworkSettings -> Networks -> bridge -> IPAddress"; which can be obtained via the `docker inspect` command)
- ping 172.17.0.2
- ip addr
- 
    1. wget -o- http://localhost:4003
    2. docker exec -it <container_id> /bin/sh
- docker stop «container_id»
- docker image rm «image_id» //delete image
- docker inspect tvs-ex1 | less //consult layers created for the image

# Ex2  docker-compose.yml
Please create the folder `elasticsearch_db_volume` first, using the host machine not by letting docker doing it first, or the Group Permissions will be: r-x (read, execute), opposed when doing it with the host machine: rwx (read, write, execute). Which would result the docker throwing the error: `fatal exception while booting failed to obtain node locks, tried [/usr/share/elasticsearch/data]; maybe these locations are not writable or multiple nodes were started on the same data`

## How to run
- docker compose up -d

Scaling command:
- docker compose up -d --scale webapp=2

## Exercise notes:
docker container ls 👇
```bash
docker container ls
CONTAINER ID   IMAGE                  COMMAND                  CREATED         STATUS         PORTS                  NAMES
1b051926c823   nginx:alpine           "/docker-entrypoint.…"   3 minutes ago   Up 3 minutes   0.0.0.0:2023->80/tcp   tvs-g03-entry-1
599614939e46   elasticsearch:8.11.1   "/bin/tini -- /usr/l…"   3 minutes ago   Up 3 minutes   9200/tcp, 9300/tcp     datastore
99e853025ced   webapp                 "docker-entrypoint.s…"   3 minutes ago   Up 3 minutes   4003/tcp               tvs-g03-webapp-1
539e9ce0cbe0   webapp                 "docker-entrypoint.s…"   3 minutes ago   Up 3 minutes   4003/tcp               tvs-g03-webapp-2
```
### 1 - Confirm that the solution is using all the replicas to process incoming requests
Using
```bash
seq 32 | xargs -I{} curl -s http://localhost:2023/ | grep "HOST" |
sed "s/<\/\?[a-z]\+>//g" | sed "s/^[[:space:]]*//" | sort | uniq -c
```
Execution 1
```
15 HOST: 539e9ce0cbe0
17 HOST: 99e853025ced
```
Executiong 2
```
20 HOST: 539e9ce0cbe0
12 HOST: 99e853025ced
```

Execution 3
```
11 HOST: 539e9ce0cbe0
21 HOST: 99e853025ced
```

### 2 - Check service logs for the solution
NGINX logs upon receiving requests (`docker logs 1b051926c823`)
```bash
2023-12-20 13:37:25 172.29.0.1 - - [20/Dec/2023:13:37:25 +0000] "GET / HTTP/1.1" 200 819 "-" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36" "-"
2023-12-20 13:37:25 172.29.0.1 - - [20/Dec/2023:13:37:25 +0000] "GET /files/tvs.css HTTP/1.1" 304 0 "http://localhost:2023/" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36" "-"
2023-12-20 13:37:25 172.29.0.1 - - [20/Dec/2023:13:37:25 +0000] "GET /files/isel.png HTTP/1.1" 304 0 "http://localhost:2023/" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36" "-"
2023-12-20 13:37:25 172.29.0.1 - - [20/Dec/2023:13:37:25 +0000] "GET /files/tvs.png HTTP/1.1" 304 0 "http://localhost:2023/" "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36" "
```

### 3 - Run a shell in any container in the solution to get its IP address and observe the running processes
We choose container webapp with ID = 99e853025ced
1. docker exec -it 99e853025ced /bin/sh (enters container's shell)
2. ip addr (command `ifconfig` can also be used, has a similar output)
3. output (IP's can be found in link/ether):
``` bash
ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: tunl0@NONE: <NOARP> mtu 1480 qdisc noop state DOWN qlen 1000
    link/ipip 0.0.0.0 brd 0.0.0.0
3: ip6tnl0@NONE: <NOARP> mtu 1452 qdisc noop state DOWN qlen 1000
    link/tunnel6 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00 brd 00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00
96: eth0@if97: <BROADCAST,MULTICAST,UP,LOWER_UP,M-DOWN> mtu 1500 qdisc noqueue state UP 
    link/ether 02:42:ac:1c:00:02 brd ff:ff:ff:ff:ff:ff
    inet 172.28.0.2/16 brd 172.28.255.255 scope global eth0
       valid_lft forever preferred_lft forever
102: eth1@if103: <BROADCAST,MULTICAST,UP,LOWER_UP,M-DOWN> mtu 1500 qdisc noqueue state UP 
    link/ether 02:42:ac:1d:00:03 brd ff:ff:ff:ff:ff:ff
    inet 172.29.0.3/16 brd 172.29.255.255 scope global eth1
       valid_lft forever preferred_lft forever
```

#### Alternative method 1
- docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' 99e853025ced
- output: 172.28.0.2172.29.0.3
#### Alternative method 2
- docker inspect 99e853025ced | grep  "IPAddress"
```bash
"SecondaryIPAddresses": null,
"IPAddress": "",
        "IPAddress": "172.28.0.2",
        "IPAddress": "172.29.0.3",
```
#### Alternative method 3
- inside nginx's container:
- nslookup webapp

#### Running processes
Methods:
1. docker top 99e853025ced
```c
UID                 PID                 PPID                C                   STIME               TTY                 TIME                CMD
1000                24457               24379               0                   13:27               ?                   00:00:00            node tvsapp.js
1000                26757               24379               0                   13:44               ?                   00:00:00            /bin/sh
```
2. docker exec -it 99e853025ced ps -eaf (or just run `ps` inside the container's shell)
```c
PID   USER     TIME  COMMAND
    1 node      0:00 node tvsapp.js
   80 node      0:00 /bin/sh
   90 node      0:00 ps -eaf
```

### 4 - Demonstrate proper connectivity and unreachability between containers in the solution
1. Proper connectivity between nginx and webapp
- docker exec -it 1b051926c823 ping 599614939e46
```c
PING 99e853025ced (172.29.0.3): 56 data bytes
64 bytes from 172.29.0.3: seq=0 ttl=64 time=0.087 ms
64 bytes from 172.29.0.3: seq=1 ttl=64 time=0.142 ms
64 bytes from 172.29.0.3: seq=2 ttl=64 time=0.193 ms
^C
--- 99e853025ced ping statistics ---
3 packets transmitted, 3 packets received, 0% packet loss
round-trip min/avg/max = 0.087/0.140/0.193 ms
```
2. Unreachability between nginx and datastore
- docker exec -it 1b051926c823 ping 599614939e46
```c
ping: bad address '599614939e46'
```

### 5 - Explain and show why http://webapp:PORT is enough on nginx to reach all the replicas
Because all the replicas share the same alias "webapp". Upon running
- docker inspect 99e853025ced

We can see that we find in the JSON:
- HostConfig -> Config -> NetworkSettings -> Networks -> "tvs-g03_webapp-data" AND "tvs-g03_webapp-entry" the same `Aliases` array:
```json
"Aliases": [
    "tvs-g03-webapp-1",
    "webapp",
    "99e853025ced"
],
```
And for the other container `539e9ce0cbe0`, consulting the same array we have:
```json
"Aliases": [
    "tvs-g03-webapp-2",
    "webapp",
    "539e9ce0cbe0"
],
```

### 6 - Explain the purpose of resolver 127.0.0.11 valid=5s in the nginx configuration
- Specifies the name servers that should be employed by Nginx to resolve hostnames to IP addresses and vice-versa
- 127.0.0.11 is a DNS Server within Docker's embedded DNS resolver (in any container run `cat /etc/resolv.conf` to check it out)
- Docker utilizes 127.0.0.11 as a loopback IP address inside containers to reference the DNS server that serves as a resolver for service discovery by their names within the Docker environment. It's an internal docker IP for networking and communication between containers.
- `valid=5s` is the time the DNS query results are cached. **By setting a shorter valid duration, NGINX remains more responsive to changes in DNS configurations. It enables NGINX to adapt quickly to changes in DNS mappings, facilitating dynamic updates within the Docker network, especially in scenarios where container instances may scale up or down, and IP addresses change frequently.** After this duration, NGINX will re-query the DNS resolver for updated records.
- In essence this line ensures that NGINX uses Docker's internal DNS resolver (127.0.0.11) for DNS resolution, sets a short duration for DNS record validity, and aids in maintaining accurate and up-to-date DNS resolution within the Docker container environment

#### Sources
- [O'Reilly, Nginx HTTP Server - Fourth Edition by Martin Fjordvald](https://www.oreilly.com/library/view/nginx-http-server/9781788623551/fe72f42a-d40d-4987-b086-7abc64c6dec4.xhtml)
- ChatGPT 🤖

## Other notes:
Like the assignment paper says, only expose the port `2023`, don't expose the webapp and elasticsearch port, they still exist but only internally. In docker desktop, in tab "inspect" of the container, you will see like: "Port: 4003/tcp Not bound" for the webapp. 

Or through `docker container ps` you will for nginx in tab "PORTS":
-  0.0.0.0:2023->80/tcp 

But for the others you will see like
- 4003/tcp  

Indicating that "->" is present when the port is exposed

And elastic search already exposes by default port `9200` and `9300`. Also note how even though we expose the port 4003 in the [Dockerfile](./tvsapp/Dockerfile), it's not exposed to the host. Indicating that what's set in the docker-compose.yml has precedence over what's in the Dockerfile

Dont use [underscores](https://docs.docker.com/compose/migrate/#service-container-names)