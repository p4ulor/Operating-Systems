## 1
- Named pipes are unidirectional, meaning data can only flow in one direction. If bidirectional communication is required, you would need to create two named pipes (one for each direction), which can complicate the implementation.
- https://stackoverflow.com/a/8391269
- https://serverfault.com/questions/124517/what-is-the-difference-between-unix-sockets-and-tcp-ip-sockets

## 2
- shared memory, by itself, only provides a shared region of memory that multiple processes can access concurrently. It lacks built-in mechanisms for synchronization, signaling, and coordination between processes
- Without proper synchronization, race conditions and data inconsistencies may occur. Additional mechanisms like semaphores or mutexes are often used to coordinate access to the shared memory region.

## 3
https://unix.stackexchange.com/questions/159174/differences-between-inactive-vs-disabled-and-active-vs-enabled-services
### a)
- enabled - a service (unit) is configured to start when the system boots
- disabled - a service (unit) is configured to not start when the system boots
- active - a service (unit) is currently running.
- inactive - a service (unit) is currently not running, but may get started, i.e. become active, if something attempts to make use of the service.

### b)
1. disabled && active
2. disabled && inactive
3. enabled && active
4. enabled && inactive

## 4
- A unit configuration file whose name ends in ".socket" encodes
information about an IPC or network socket or a file system FIFO
controlled and supervised by systemd, for socket-based activation.
- Socket units on the other hand don't actually start daemons on their own. Instead, they just sit there and listen on an IP address and a port, or a UNIX domain socket, and when something connects to it, the daemon that the socket is for is started and the connection is handed to it.
- This is useful for making sure that big daemons that take up a lot of resources but are rarely used aren't running and taking up resources all the time, but instead they are only started when needed.
- https://unix.stackexchange.com/questions/159462/what-is-systemds-target-service-and-socket

## 5
- The hypervisor is the monitor of the virtual machine
- Paravirtualization is a type of virtualization where software instructions from the guest operating system running inside a virtual machine can use “hypercalls” that communicate directly with the hypervisor. This provides an interface very similar to software running natively on the host hardware.
- The main benefits of paravirtualization are where instructions are not compatible with full virtualization or where more immediate access to underlying hardware is required for performance reasons. For timing-critical functions, paravirtualization can provide the speed of native code alongside some of the benefits of virtualization, such as sharing hardware between multiple operating systems.
- https://blackberry.qnx.com/en/ultimate-guides/automotive-hypervisor/paravirtualization

## 6
- Uma máquina virtual de processo é uma máquina virtual que pretende só virtualizar um processador de uma determinada arquitetura e não um sistema operativo inteiro e sem ter de virtualizar o hardware. O principal objetivo destas máquinas virtuais é suportar binários compilados em um conjunto de instruções diferentes. Um exemplo desta maquina é o "QEMU"
- https://en.wikipedia.org/wiki/QEMU
- https://www.sciencedirect.com/topics/computer-science/process-virtual-machine

## 7
- Docker never uses a different kernel: the kernel is always your host kernel if you're using a Linux distribution or the kernel version used in WSL if you're using windows.
- If your kernel is "compatible enough" with the software in the container you want to run it will work; otherwise, it won't.
- Docker can't run correctly if your kernel is older than version 3.10, so the comment is wrong.
- https://docs.docker.com/engine/install/troubleshoot/
- https://stackoverflow.com/questions/32841982/how-can-docker-run-distros-with-different-kernels
- https://en.wikipedia.org/wiki/Docker_(software)#Background
- https://docs.docker.com/engine/faq/

## 8
### a)
- Novas camadas de overlay sao criadas ao executar todos (o EXPOSE cria uma camada vazia) os comandos num Dockerfile, principalmente aqueles que alteram o filesystem, por exemplo ao executar COPY e RUN
- https://stackoverflow.com/questions/41637560/does-docker-expose-make-a-new-layer
- https://docs.docker.com/build/cache/
### b)
- Existe reaproveitamento, se existem layers iguais em cache. Para o fazer de forma personalizada, deve-se agrupar comandos. Por exemplo `RUN apt-get update && apt-get upgrade -y`
- https://docs.docker.com/build/cache/

## 9
The observable difference is that pinging "svcb" will resolve to the IP address of any instance of the svcb service, while pinging "tp2-svcb-1" will target the specific container with that name. If you want to target a particular instance of the svcb service, you should use the container name (tp2-svcb-1, for example). If you want to target any instance of svcb, you can use the service name ("svcb").
