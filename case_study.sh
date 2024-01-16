#! /bin/bash

prj=$1

if [  -z ${prj} ]
then
 echo "Remember to add the name of the project you want to compute"
else
(
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR=${SCRIPT_DIR}/build
EXE_FILE=${BUILD_DIR}/phreesqlexe

if [ ! -f ${EXE_FILE} ]
then
	${SCRIPT_DIR}/build.sh
fi

${SCRIPT_DIR}/example/00_step-by-step.sh ${prj}

)

fi
