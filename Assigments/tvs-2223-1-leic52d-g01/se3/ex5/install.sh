#!/bin/bash

TVSCTL_EXE=./tvsctl
TVSCTL_SRC=./tvsctl.c

if [ ! -x $TVSCTL_EXE ]; then
	if [ ! -f $TVSCTL_SRC ]; then
		echo Wrong directory
	else
		echo Run make before install
	fi
	exit
fi

TVSCTL_DIR=/opt/isel/tvs/tvsctl/bin
SYSTD_DIR=/etc/systemd/system

mkdir -p $TVSCTL_DIR

if [ ! -d $TVSCTL_DIR ]; then
	echo Failed to create $TVSCTL_DIR
	exit
fi

cp $TVSCTL_EXE $TVSCTL_DIR
cp ./tvsctl.socket  $SYSTD_DIR
cp ./tvsctl.service $SYSTD_DIR

if [ -x $TVSCTL_DIR/$TVSCTL_EXE ] && [ -f $SYSTD_DIR/tvsctl.socket ] && [ -f $SYSTD_DIR/tvsctl.service ]; then
	systemctl enable tvsctl.socket
else
	echo Failed to copy tvsctl files
	exit
fi


