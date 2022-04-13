#!/bin/sh

set -eu

BUILDDIR=${BUILDDIR:-"./.build"}
BUILDTYPE=${BUILDTYPE:-"debug"}

fast_build=0

usage()
{
	echo "Usage: ${0} [OPTIONS]"
	echo "Build and compile whole project."
	echo "Available options:"
	echo "    -h   Print this help (usage)."
	echo "    -f   Incremental build."
}

cleanup() {
	trap EXIT
}

main()
{
	_start_time="$(date "+%s")"

	while getopts ":hf" _options; do
		case "${_options}" in
		h)
			usage
			exit 0
			;;
		f)
			fast_build=1
			echo "info" "Using lightening speed..."
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

	if [ $fast_build = 0 ]; then
		rm -rf "${BUILDDIR}"
		meson setup	--buildtype "${BUILDTYPE}" "${BUILDDIR}"
	fi

	meson compile -C "${BUILDDIR}"

	cleanup

	echo "==============================================================================="
	echo "Ran build and compile processes in $(($(date "+%s") - _start_time)) seconds"
	echo "==============================================================================="
}

main "${@}"

exit 0
