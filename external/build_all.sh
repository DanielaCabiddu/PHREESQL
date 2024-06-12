#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

BUILD_FOLDER=${SCRIPT_DIR}/../build

echo ${BUILD_FOLDER}
mkdir -p ${BUILD_FOLDER} 

echo "Building IPhreeQC ... "

PHREEQC_BUILD_FOLDER=${BUILD_FOLDER}/phreeqc
PROJ_BUILD_FOLDER=${BUILD_FOLDER}/proj

echo ${PHREEQC_BUILD_FOLDER}
rm -rf ${PHREEQC_BUILD_FOLDER}
mkdir ${PHREEQC_BUILD_FOLDER}
cd ${PHREEQC_BUILD_FOLDER}
cmake ${SCRIPT_DIR}/iphreeqc-3.7.0-15749
make -j8

echo "Building IPhreeQC ... DONE"
echo "Building ProJ ... "

cd ..
echo ${PROJ_BUILD_FOLDER}
rm -rf ${PROJ_BUILD_FOLDER}
mkdir ${PROJ_BUILD_FOLDER}
cd ${PROJ_BUILD_FOLDER}
cmake ${SCRIPT_DIR}/proj-osgeo -DBUILD_TESTING=OFF
make -j8

echo "Building ProJ ... DONE"




