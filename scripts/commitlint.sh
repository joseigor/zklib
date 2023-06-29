#!/bin/sh
commitlint -x $(npm root -g)/@commitlint/config-conventional "$@"
