cmake_minimum_required(VERSION 3.5)

project(phreesql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-std=c++17 -fpermissive")

include_directories(BEFORE ../phreesqlib/)

set (EXTERNAL_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../external")

## PhreeQC paths.
## They refer to the included PhreeQC version.
## They must be properly set by users if a different version of PhreeQC is used.
set (PHREEQC_FOLDER "${EXTERNAL_FOLDER}/iphreeqc-3.7.0-15749")
set (PHREEQC_SRC_FOLDER "${PHREEQC_FOLDER}/src")
set (IPHREEQC_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/phreeqc/libIPhreeqc.a")

## Automatic PhreeQC building approach.
## It refers to the included PhreeQC version.
## Disable if a different version of PhreeQC is used.
execute_process (
    WORKING_DIRECTORY ${EXTERNAL_FOLDER}
    COMMAND /bin/bash -c "./build_all.sh"
)

include_directories(BEFORE ${PHREEQC_SRC_FOLDER})
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/common)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/PhreeqcKeywords)

include_directories(BEFORE ${EXTERNAL_FOLDER}/file-joiner-2021.07.16-11.12/src)

add_executable(phreesql main.cpp)


target_link_libraries(${PROJECT_NAME} ${IPHREEQC_LIB} sqlite3)
