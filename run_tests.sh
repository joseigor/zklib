#!/bin/sh

set -eu

BUILDDIR=${BUILDDIR:-"./.build"}
UNIT_TEST_REPORT_PATH_DEF="${BUILDDIR}/artifacts/test_coverage_report"
UNIT_TEST_REPORT_PATH="${UNIT_TEST_REPORT_PATH_DEF}"

usage()
{
	echo "Usage: ${0} [OPTIONS]"
	echo "Run project tests."
	echo "Available options:"
	echo "    If no options passed runs all test cases."
	echo "    -t <test_name>  Runs test specified by <test_name>."
	echo "    -d <folder>     Folder to save the coverage test report."
}

cleanup() {
	trap EXIT
}

check_meson_build() {
	if [ ! -d "${BUILDDIR}" ]; then
		echo "Meson build directory \"${BUILDDIR}\" is missing, build project with coverage enabled (./build.sh -g ) before testing."
		exit 1
	fi
}

run_tests() {

	check_meson_build

	meson \
		test \
		-v \
		 --wrap='
		 	valgrind
				--leak-check=full
				--show-leak-kinds=all
				--errors-for-leak-kinds=all
				--keep-stacktraces=alloc-and-free
				--read-var-info=yes
				--track-origins=yes
				--errors-for-leak-kinds=definite,possible
				--leak-resolution=high
				--num-callers=40
				--expensive-definedness-checks=yes
				--show-mismatched-frees=yes
				--xtree-memory=full
				--xtree-leak=yes
				--xtree-leak-file=xtleak.kcg.%p
				--error-exitcode=1
			' \
		 -C "${BUILDDIR}" "${TEST_NAME:-}"

	if [ ! -d "$UNIT_TEST_REPORT_PATH_DEF" ]; then
		mkdir -p "$UNIT_TEST_REPORT_PATH_DEF"
	fi

	gcovr \
		-r ./src \
		./.build/src \
		--html-details "${UNIT_TEST_REPORT_PATH_DEF}/unit_test_coverage_results.html" \
		--txt

	if [ "${UNIT_TEST_REPORT_PATH}" != "${UNIT_TEST_REPORT_PATH_DEF}" ]; then
		mkdir -p "${UNIT_TEST_REPORT_PATH}"
		cp -r "./${UNIT_TEST_REPORT_PATH_DEF}" "${UNIT_TEST_REPORT_PATH}"
	fi

	echo "******************************************************************************************"
	echo "Please open file: ${UNIT_TEST_REPORT_PATH}test_coverage_report/unit_test_coverage_results.html to see actual unit test report."
	echo "******************************************************************************************"




}

main()
{
	while getopts ":ht:d:" _options; do
		case "${_options}" in
		h)
			usage
			exit 0;
			;;

		t)
			TEST_NAME="${OPTARG:-}"
			;;

		d)
			UNIT_TEST_REPORT_PATH="${OPTARG:-UNIT_TEST_REPORT_PATH_DEF}"
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

	shift "$((OPTIND - 1))"

	run_tests "${@}"

	cleanup
}

main "${@}"

exit 0

