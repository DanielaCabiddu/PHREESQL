#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

BUILD_FOLDER=${SCRIPT_DIR}/../build

echo ${BUILD_FOLDER}
mkdir -p ${BUILD_FOLDER} 

echo "Building IPhreeQC ... "

PHREEQC_BUILD_FOLDER=${BUILD_FOLDER}/phreeqc

echo ${PHREEQC_BUILD_FOLDER}
rm -rf ${PHREEQC_BUILD_FOLDER}
mkdir ${PHREEQC_BUILD_FOLDER}
cd ${PHREEQC_BUILD_FOLDER}
cmake ${SCRIPT_DIR}/iphreeqc-3.7.0-15749
make -j8

echo "Building IPhreeQC ... DONE"



