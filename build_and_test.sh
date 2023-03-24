#!/bin/bash

(
set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_SCRIPT=${SCRIPT_DIR}/build.sh
TEST_SCRIPT=${SCRIPT_DIR}/test.sh

${BUILD_SCRIPT}

${TEST_SCRIPT}

)
