## 6
Unix domain sockets support simultaneous connections and allow multiple clients to communicate with a server concurrently.

- Each connected client can be assigned a unique identifier or context to distinguish one client from another.
- This identifier can be associated with each client's connection, allowing the server to keep track of different clients.
## 7
Daemon processes in a Linux system, often associated with background services, exhibit several characteristics that distinguish them from regular processes

1. **Background Execution:**
   - Daemon processes operate in the background, detached from the terminal session that started them. They do not have a controlling terminal, allowing them to continue running even if the user logs out or the terminal session ends.

2. **No User Interaction:**
   - Daemons typically run without any direct user interaction. They operate silently in the background, performing specific tasks or providing services without requiring input from the user.
   - They may be configured to start automatically during system boot and continue running throughout the system's uptime.

3. **Independent Lifecycle:**
   - Daemons have an independent lifecycle and can run continuously, waiting for events or performing periodic tasks. They are not tied to the execution of a specific user or terminal session.
   - Daemons often use features like signal handling to respond to specific events (e.g., SIGHUP to reload configuration) and can be started, stopped, or restarted independently of other system processes.

These characteristics make daemon processes well-suited for long-running tasks, background services, and system-level operations where continuous and autonomous execution is required.

## 8
### a)
funcionalidade -> Socket activation. The presence of a .socket unit suggests that socket activation might be in use. Socket activation is a systemd feature that allows services to be started on-demand when a connection is received on a socket, rather than starting the service upfront.

Proposito do require -> criaçao de dependencia. Ensure that when tp2.service is started, the associated socket (presumably used for communication or activation) is also available and ready. This is particularly useful in scenarios where tp2.service relies on incoming connections or events triggered by the corresponding socket.

### b)
No, it is generally not necessary or common to have an equivalent Requires directive in the .socket file pointing to the corresponding .service file. The reason is that socket activation, by its nature, doesn't require the service to be started beforehand.

## 9
- O hipervisor tipo 1, ou hipervisor bare metal, interage diretamente com o hardware subjacente da máquina. O hipervisor bare metal é instalado diretamente no hardware físico da máquina host, não por meio de um sistema operacional. Em alguns casos, o hipervisor tipo 1 é incorporado ao firmware da máquina. 
- Como todo o hardware vai ser escrito por cima pelo hipervisor, parece ser do tipo 1
- https://aws.amazon.com/pt/compare/the-difference-between-type-1-and-type-2-hypervisors/

## 10
Vantagens:
1. Reaprovamento do processamento (e tempo) do build que foi feito previamente se houver layers semelhantes que foi mantidas em cache
2. Imagens mais pequenas (menos layers)

Desvantagem:
- Porque o docker tenta usar o que esta em cache, as vezes nao sao executados comandos que deviam ter sido. Por exemplo `RUN apt-get update` nao ser executado. https://docs.docker.com/develop/develop-images/instructions/#run
- Chaining RUN instructions can reduce readability (unless backslashes are used)

## 11
1. É criada 1 camada
2. 
```Dockerfile
FROM node:alpine
WORKDIR /home/node
COPY package.json . && COPY app.js . COPY README.md .
RUN npm install && RUN chown -R node.node /home/node
USER node
EXPOSE 80
CMD ["node", "app,js"]
```
3. 7

## 12
Igual ao tp2_e0-easy

The observable difference is that nslookup "svcb" will resolve to the IP address of any instance of the svcb service, while nslookup "tp2-svcb-1" will target the specific container with that name. If you want to target a particular instance of the svcb service, you should use the container name (tp2-svcb-1, for example). If you want to target any instance of svcb, you can use the service name ("svcb").
