.PHONY: all clean

all: tvsctl tvsctl-cli

tvsctl: tvsctl.c /usr/include/systemd/sd-daemon.h
	gcc -Wall -o tvsctl tvsctl.c -lsystemd

tvsctl-cli: ../ex6/tvsctl-cli.c
	gcc -Wall -o tvsctl-cli ../ex6/tvsctl-cli.c

/usr/include/systemd/sd-daemon.h:
	sudo apt install libsystemd-dev

clean:
	rm -f tvsctl tvsctl-cli