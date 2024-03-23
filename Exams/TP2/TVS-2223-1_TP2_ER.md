## 6
- The sockaddr_un structure is typically used for Unix domain sockets, but the code is attempting to use it as if it were a network socket structure.
- O cliente tem variaveis com nome que respeitam o servidor, mas com nome de cliente
- Em sprintf é escrito tp2-client e nao tp2-servidor
- The client address (cli_addr) should be set to the server's address, not a dynamically generated client address when performing the connect()

## 7
O systemctl stop «nome do serviço» vai enviar um sinal SIGTERM para o processo principal do serviço para tentar parar esse serviço graciosamente. Se necessário o utilizador também poderá usar o SIGKILL. Não sendo definido a diretiva ExecStop, este será o comportamento por default e nao o definido pelo programa especificado em ExecStop

## 8
- RequiredBy may be suitable when a service is truly critical for the operation of a target, and the target shouldn't reach the active state without the required service being active. If the required unit fails to start or is not present, it will prevent the target from reaching its active state.
- WantedBy is often chosen when the service is important but not strictly required, allowing for more leniency in system initialization. It means that the unit is intended to be started when the target is started, but its absence does not prevent the target from reaching the active state.

In many scenarios, it's preferable to use WantedBy because it provides a more flexible and forgiving relationship between units. If a service is wanted by a target and encounters issues during startup, it won't block the activation of the target, allowing other services to proceed.

## 9
Paravirtualização

The intent of the modified interface is to reduce the portion of the guest's execution time spent performing operations which are substantially more difficult to run in a virtual environment compared to a non-virtualized environment. The paravirtualization provides specially defined 'hooks' to allow the guest(s) and host to request and acknowledge these tasks, which would otherwise be executed in the virtual domain (where execution performance is worse). A successful paravirtualized platform may allow the virtual machine monitor (VMM) to be simpler (by relocating execution of critical tasks from the virtual domain to the host domain), and/or reduce the overall performance degradation of machine execution inside the virtual guest.

## 10
https://docs.docker.com/get-started/overview/#example-docker-run-command
1. **Image Pull (if not available locally):**
If the `ubuntu:latest` image is not already available locally, Docker will fetch it from the Docker Hub or the configured container registry. This involves downloading the necessary layers that compose the Ubuntu image.

2. **Container Creation:**
Docker creates a new container based on the specified image. This involves setting up the container file system, network configuration, and other container-specific settings. As though you had run a docker container create command manually.

3. **Namespace Isolation:**
Docker uses Linux namespaces to isolate the container from the host system. Namespaces provide separate views of resources such as process IDs, network, mount points, and more. 
- Allocates a read-write filesystem to the container, as its final layer. This allows a running container to create or modify files and directories in its local filesystem.
- Docker creates a network interface to connect the container to the default network, since you didn't specify any networking options. This includes assigning an IP address to the container. By default, containers can connect to external networks using the host machine's network connection.

4. **Container Initialization:**
Docker initiates the container by running its default command, which, in this case, is determined by the `CMD` instruction in the Ubuntu image. Since you used the `-it` flags, the container starts an interactive shell within the container.

5. **Terminal Interaction:**
Docker starts the container and executes /bin/bash. Because the container is running interactively and attached to your terminal (due to the -i and -t flags), you can provide input using your keyboard while Docker logs the output to your terminal.

6. **Process Execution and Isolation:**
The container's user space is executed as an isolated process on the host system. The processes inside the container are separate from those on the host, thanks to namespace isolation.

7. **Resource Management:**
Docker employs control groups (cgroups) to manage and limit system resources (CPU, memory, etc.) allocated to the container. Resource constraints specified during container creation are enforced by the Docker runtime.

## 11
To start multiple containers for the svc service, you can use the --scale option with the docker-compose up command.
- docker-compose up --scale svc=3

This command will start three containers of the svc service. Each container will be connected to the tp2net network

Regarding reaching the containers on the tp2net network, Docker Compose uses service names as DNS names for the containers within the same network. In this case, you can use the service name (svc) as the hostname to reach any container within the network. Or a specific container with `tp2-svc-X` where X is the number of the container

## 12
```Dockerfile
FROM node:alpine

WORKDIR /home/node/webapp

# Copy only the necessary files first to optimize cache reuse
COPY package.json .
COPY images ./images
COPY README.md .

# Change ownership after copying files
RUN chown -R node.node /home/node

USER node
ENV PORT=80

COPY app.js . && RUN npm install

EXPOSE $PORT

CMD ["node", "app.js"]
```

### Changes Made:

1. **Corrected CMD Directive:**
   - Changed `"node", "app,js"` to `"node", "app.js"`.

2. **Optimized COPY Commands:**
   - Rearranged the `COPY` commands to copy only necessary files first to optimize cache reuse. This way, changes in `package.json` and `images` folder won't invalidate the entire cache.

3. **Changed Order of Commands:**
   - Changed the order of commands to perform `chown` after copying files, ensuring ownership is set correctly for all files, including the ones added during the build.

By making these changes, the Dockerfile should align with the stated objectives, ensuring that the ownership is correctly set, optimizing cache reuse, and minimizing the number of overlay layers.