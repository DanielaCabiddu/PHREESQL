cmake_minimum_required(VERSION 3.21)

project(phreesqlexe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-std=c++17 -fpermissive")

#option(BUILD_DOC "Build documentation" OFF)

find_package(OpenMP)

if (NOT OPENMP_FOUND)
    #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    message ("OpenMP not found. Parallelism DISABLED.")
endif()

include_directories(BEFORE ../phreesqlib/)

set (EXTERNAL_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../external")

## PhreeQC paths.
## They refer to the included PhreeQC version.
## They must be properly set by users if a different version of PhreeQC is used.
set (PHREEQC_FOLDER "${EXTERNAL_FOLDER}/iphreeqc")
set (PHREEQC_SRC_FOLDER "${PHREEQC_FOLDER}/src")
set (IPHREEQC_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/phreeqc/libIPhreeqc.a")

add_custom_command(
  OUTPUT "${IPHREEQC_LIB}"
  COMMAND mkdir -p build && cd build && mkdir -p phreeqc && cd phreeqc && 
        cmake ${PHREEQC_FOLDER} -DCMAKE_CXX_STANDARD=11 -DBUILD_TESTING=OFF -DIPHREEQC_ENABLE_MODULE=OFF &&
        cmake --build .
  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
)

set (PROJ_FOLDER "${EXTERNAL_FOLDER}/proj-osgeo")
set (PROJ_SRC_FOLDER "${PROJ_FOLDER}/src")

if(APPLE)
    set (PROJ_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/proj/lib/libproj.dylib")
else()
    set (PROJ_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../build/proj/lib/libproj.so")
endif()

add_custom_command(
  OUTPUT "${PROJ_LIB}"
  COMMAND mkdir -p build && cd build && mkdir -p proj && cd proj && 
        cmake ${PROJ_FOLDER} -DBUILD_TESTING=OFF -DENABLE_CURL=OFF -DENABLE_TIFF=OFF -DBUILD_APPS=OFF &&
        cmake --build .
  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
)

include_directories(BEFORE ${PHREEQC_SRC_FOLDER})
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/common)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/PhreeqcKeywords)

include_directories(BEFORE ${PROJ_SRC_FOLDER})

if (BUILD_DOC)
    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    else (DOXYGEN_FOUND)
      message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    ${IPHREEQC_LIB}
    ${PROJ_LIB}
)

list(APPEND EXTRA_LIBS ${IPHREEQC_LIB})
list(APPEND EXTRA_LIBS "sqlite3")
list(APPEND EXTRA_LIBS ${PROJ_LIB})

if (OPENMP_FOUND)
    list(APPEND EXTRA_LIBS "OpenMP::OpenMP_CXX")
endif()

message (${PROJ_LIB})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS} )

get_target_property(OUT ${PROJECT_NAME} LINK_LIBRARIES)
message(STATUS "ldd:  ${OUT}")
