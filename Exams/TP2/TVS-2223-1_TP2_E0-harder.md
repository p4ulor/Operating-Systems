## 1
- The code lacks proper error handling. For example, if the mkfifo or open operations fail (returning -1), the code continues to execute without handling these errors, leading to potential issues.
- The memcpy(data, &req[1], 4); operation copies 4 bytes from the req buffer to the data buffer without checking if there are enough bytes available in req. This can lead to a buffer overflow if the received message is not properly formatted.

## 2


## 3
- A informaçao é obtida pelos ficheiros com extensão .service em "/etc/systemd/system". Sabendo os serviços existentes, é usado o programa `systemctl` para deterinar o estado do serviço. Por exemplo via `systemctl is-enabled elasticsearch.service`

## 4
### a)


### b)

## 5


## 6
Se o OS que estiver a ser usado for uma distribuiçao do Linux, o kernel vai ser o mesmo para as system calls. Para o windows e o mac, é usada uma pequena maquina virtual de Linux (WSL para windows). Os containers correm dentro desta (pequena) maquina virtual usando o kernel do linux

- https://stackoverflow.com/questions/32756988/what-is-meant-by-shared-kernel-in-docker

## 7
### a)
the asterisk wildcard doesnt work in Docker
- https://stackoverflow.com/a/35809234
- https://docs.docker.com/engine/reference/commandline/container_cp/

### b)
```Dockerfile
FROM ubuntu
WORKDIR /opt/isel/tp2

COPY package.json /opt/isel/tp2/
COPY app.js /opt/isel/tp2/

RUN apt update && apt install -y npm nodejs && npm install
EXPOSE 80
CMD ["node", "app.js"]
```

## 8
Docker provides a container runtime environment that isolates processes and their dependencies, including network configurations. Here's how this coexistence is achieved:

1. **Isolation through Containers:**
   - Each Docker container represents an isolated environment that includes its own file system, processes, network namespace, and other resources.
   - Containers share the host kernel but have their own isolated user space, providing a level of process and resource isolation.

2. **Network Namespace Isolation:**
   - Docker uses Linux network namespaces to provide network isolation between containers.
   - Each container has its own network namespace, which includes network interfaces, routing tables, and network-related resources.
   - Containers within the same service (defined in Docker Compose) can communicate with each other using the service's internal network, but they are isolated from containers in other services.


