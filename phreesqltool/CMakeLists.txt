cmake_minimum_required(VERSION 3.5)

project(phreesql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive")

include_directories(BEFORE ../phreesqlib/)

include_directories(BEFORE ../external/iphreeqc-3.7.0-15749/src/)
include_directories(BEFORE ../external/iphreeqc-3.7.0-15749/src/phreeqcpp)
include_directories(BEFORE ../external/iphreeqc-3.7.0-15749/src/phreeqcpp/common)
include_directories(BEFORE ../external/iphreeqc-3.7.0-15749/src/phreeqcpp/PhreeqcKeywords)

include_directories(BEFORE ../external/file-joiner-2021.07.16-11.12/src)

add_executable(phreesql main.cpp)

set(IPHREEQC_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/phreeqc/libIPhreeqc.a")

target_link_libraries(${PROJECT_NAME} ${IPHREEQC_LIB} sqlite3)
