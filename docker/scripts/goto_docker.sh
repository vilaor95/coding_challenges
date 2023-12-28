#!/bin/bash

DOCKER_IMAGE="devel"
IMAGE_TAG="latest"

{ [[ -d "$1" ]] && DEPLOY_PATH="$1" && shift;} || DEPLOY_PATH="/tmp"

docker run -it --rm \
	--privileged \
	-e LOCAL_USER_ID=`id -u` \
	-e LOCAL_USER_NAME=`whoami` \
	-e TERM=xterm-256color \
	-v /dev/bus/usb:/dev/bus/usb \
	-v ~/.ssh:/home/`whoami`/.ssh:ro \
	-v ~/.cache:/home/`whoami`/.cache \
	-v ~/.config:/home/`whoami`/.config:ro \
	-v `pwd`:/home/developer/workspace \
	-v ${DEPLOY_PATH}:/home/developer/deploy \
	-w /home/developer/workspace \
	${DOCKER_IMAGE}:${IMAGE_TAG}
