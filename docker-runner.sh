#!/bin/sh

set -eu


BUILD_IMAGE_NAME=${BUILD_IMAGE_NAME:-"zklib"}
BUILD_CONTAINER_NAME=${BUILD_CONTAINER_NAME:-"zklib-builder-container"}

DEBUGGER_IMAGE_NAME=${DEBUGGER_IMAGE_NAME:-"zklib-debugger"}
DEBUGGER_CONTAINER_NAME=${DEBUGGER_CONTAINER_NAME:-"zklib-debugger-container"}


usage()
{
	echo "Usage: ${0} [OPTIONS]"
	echo "Build docker images and creates docker conainters."
	echo "Available options:"
	echo "    -b   Build docker image and container for BUILD purpouse."
	echo "    -d   Build docker image and container for DEBUG purpouse."
	echo "    -e   Run build container."
	echo "    -f   Run debugger container."
	echo "    -r   Remove all images and containers."
}

cleanup() {
	trap EXIT
}

create_builder_container() {

	echo "info" "Will create builder contianer and run in detached mode."

	docker build -t "${BUILD_IMAGE_NAME}" .

	if [ "$(docker ps -a -q -f name="${BUILD_CONTAINER_NAME}")" ]; then
		docker rm -f "${BUILD_CONTAINER_NAME}" > /dev/null
	fi

	docker \
		run \
		-it \
		-v $(pwd):/workdir \
		--name "${BUILD_CONTAINER_NAME}" \
		-d "${BUILD_IMAGE_NAME}"

	echo "info" "Builder contianer running in detached mode"

}

run_builder_container() {

	echo "info" "Will run builder docker container."

	# checks if container exists
	if [ ! "$(docker ps -a -q -f name="${BUILD_CONTAINER_NAME}")" ]; then
		create_builder_container
	fi

	# checks if container is running
	if [ ! "$(docker ps -q -f status=running -f name="${BUILD_CONTAINER_NAME}")" ]; then
		docker start "${BUILD_CONTAINER_NAME}" > /dev/null
	fi

	docker exec -it "${BUILD_CONTAINER_NAME}" /bin/bash

	echo "info" "Exited from builder docker container."

}

create_debugger_container() {

	echo "info" "Will create debugger contianer and run in detached mode."

	docker build -t "${BUILD_IMAGE_NAME}" .
	docker build -t "${DEBUGGER_IMAGE_NAME}" -f Dockerfile.debug .

	if [ "$(docker ps -q -f name="${DEBUGGER_CONTAINER_NAME}")" ]; then
		docker rm -f "${DEBUGGER_CONTAINER_NAME}" > /dev/null
	fi

	docker \
		run \
		-it \
		--privileged \
		-v $(pwd):/workdir \
		--name "${DEBUGGER_CONTAINER_NAME}" \
		-d "${DEBUGGER_IMAGE_NAME}"

	echo "info" "Debugger contianer running in detached mode."
}

run_debugger_container() {

	echo "info" "Will run debugger docker container."

	# checks if container exists
	if [ ! "$(docker ps -a -q -f name="${DEBUGGER_CONTAINER_NAME}")" ]; then
		create_debugger_container
	fi

	# checks if container is running
	if [ ! "$(docker ps -q -f status=running -f name="${DEBUGGER_CONTAINER_NAME}")" ]; then
		docker start "${DEBUGGER_CONTAINER_NAME}" > /dev/null
	fi

	docker exec -it "${DEBUGGER_CONTAINER_NAME}" /bin/bash

	echo "info" "Exited from debugger docker container."
}

docker_clean_all() {

	echo "info" "Will remove all docker containers and images."

	# remove builder and debugger containers
	if [ "$(docker ps -q -f status=running -f name="${BUILD_CONTAINER_NAME}")" ]; then
		echo "Stoping container "${BUILD_CONTAINER_NAME}"."
		docker stop "${BUILD_CONTAINER_NAME}" > /dev/null
		echo "Container ${BUILD_CONTAINER_NAME} stoped."
	fi

	if [ "$(docker ps -a -q -f name="${BUILD_CONTAINER_NAME}")" ]; then
		echo "Removing container "${BUILD_CONTAINER_NAME}"."
		docker rm "${BUILD_CONTAINER_NAME}" > /dev/null
		echo "Container "${BUILD_CONTAINER_NAME}" removed."
	fi

	if [ "$(docker ps -q -f status=running -f name="${DEBUGGER_CONTAINER_NAME}")" ]; then
		echo "Stoping container "${DEBUGGER_CONTAINER_NAME}"."
		docker stop "${DEBUGGER_CONTAINER_NAME}" > /dev/null
		echo "Container ${DEBUGGER_CONTAINER_NAME} stoped."
	fi

	if [ "$(docker ps -a -q -f name="${DEBUGGER_CONTAINER_NAME}")" ]; then
		echo "Removing container "${DEBUGGER_CONTAINER_NAME}"."
		docker rm "${DEBUGGER_CONTAINER_NAME}" > /dev/null
		echo "Container "${DEBUGGER_CONTAINER_NAME}" removed."
	fi

	# remove builder and debugger images
	# Order is important as debugger image depends on builder image. First remove debugger image
	if [ "$(docker images -q "${DEBUGGER_IMAGE_NAME}")" ]; then
		echo "Removing image "${DEBUGGER_IMAGE_NAME}"."
		docker rmi "${DEBUGGER_IMAGE_NAME}" > /dev/null
		echo "Image ${DEBUGGER_IMAGE_NAME} removed."
	fi

	if [ "$(docker images -q "${BUILD_IMAGE_NAME}")" ]; then
		echo "Removing image "${BUILD_IMAGE_NAME}"."
		docker rmi "${BUILD_IMAGE_NAME}" > /dev/null
		echo "Image ${BUILD_IMAGE_NAME} removed."
	fi

	echo "info" "All  docker containers and images removed."
}

main()
{
	while getopts ":hbdefr" _options; do
		case "${_options}" in
		h)
			usage
			exit 0
			;;
		b)
			create_builder_container
			;;

		d)
			create_debugger_container
			;;

		e)
			run_builder_container
			;;
		f)
			run_debugger_container
			;;

		r)
			docker_clean_all
			;;

		:)
			echo "Option -${OPTARG} requires an argument."
			exit 1
			;;
		?)
			echo "Invalid option: -${OPTARG}"
			exit 1
			;;
		esac
	done

	cleanup
}

main "${@}"

exit 0

