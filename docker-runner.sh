#!/bin/sh

set -eu

IMAGE_NAME=${IMAGE_NAME:-"zklib"}
CONTAINER_NAME=${CONTAINER_NAME:-"zklib-container"}

# build docker image
docker build -t "${IMAGE_NAME}" .

# start docker container
if [ ! "$(docker ps -q -f name="${CONTAINER_NAME}")" ]; then
    if [ "$(docker ps -aq -f status=exited -f name="${CONTAINER_NAME}")" ]; then
        # cleanup
        docker rm "${CONTAINER_NAME}"
    fi
    # run your container
    docker run -it  -v $(pwd):/workdir --name "${CONTAINER_NAME}" zklib
fi
