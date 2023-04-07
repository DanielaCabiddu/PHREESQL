#! /bin/bash

(
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=${SCRIPT_DIR}/build
EXE_FILE=${BUILD_DIR}/phreesqlexe

if [ ! -f ${EXE_FILE} ]
then
	${SCRIPT_DIR}/build.sh
fi

${SCRIPT_DIR}/example/00_step-by_step.sh

)
