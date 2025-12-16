cmake_minimum_required(VERSION 3.21)

project(phreesqlexe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_BUILD_TYPE Release CACHE STRING "")
set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "")

set(CMAKE_CXX_FLAGS "-std=c++17 -fpermissive")

option(USE_OPENMP "Use OpenMP" ON)
#option(BUILD_DOC "Build documentation" OFF)

if (USE_OPENMP)
    find_package(OpenMP)

    if (NOT OPENMP_FOUND)
        #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        message ("OpenMP not found. Parallelism DISABLED.")
    endif()
else()
    message ("OpenMP DISABLED.")
endif()

include_directories(BEFORE ../phreesqlib/)

set (EXTERNAL_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/../external")

set (BUILD_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../build)

file (MAKE_DIRECTORY ${BUILD_DIRECTORY})

## PhreeQC paths.
## They refer to the included PhreeQC version.
## They must be properly set by users if a different version of PhreeQC is used.
set (PHREEQC_FOLDER "${EXTERNAL_FOLDER}/iphreeqc")
set (PHREEQC_SRC_FOLDER "${PHREEQC_FOLDER}/src")

set (PHREEQC_BUILD_DIRECTORY ${BUILD_DIRECTORY}/phreeqc)
file (MAKE_DIRECTORY ${PHREEQC_BUILD_DIRECTORY})

IF (MSVC)
    set (IPHREEQC_LIB "${PHREEQC_BUILD_DIRECTORY}/Release/IPhreeqc.lib")
ELSE()
    set (IPHREEQC_LIB "${PHREEQC_BUILD_DIRECTORY}/libIPhreeqc.a")
ENDIF()

add_custom_command(
  OUTPUT "${IPHREEQC_LIB}"
  COMMAND
        cmake ${PHREEQC_FOLDER} -DCMAKE_CXX_STANDARD=11 -DBUILD_TESTING=OFF -DIPHREEQC_ENABLE_MODULE=OFF &&
        cmake --build . --config Release --parallel 16
  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY "${PHREEQC_BUILD_DIRECTORY}"
)


find_package(SQLite3)

if(NOT SQLITE3_FOUND AND MSVC)

    if(NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
        if(NOT DEFINED ENV{VCPKG_ROOT})
            if(WIN32)
                set(VCPKG_ROOT $ENV{HOMEDRIVE}$ENV{HOMEPATH}/vcpkg_cpptruths)
            else()
                set(VCPKG_ROOT $ENV{HOME}/.vcpkg_cpptruths)
            endif()
        else()
            set(VCPKG_ROOT $ENV{VCPKG_ROOT})
        endif()

        if(NOT EXISTS ${VCPKG_ROOT})
            message("Cloning vcpkg in ${VCPKG_ROOT}")
            execute_process(COMMAND git clone https://github.com/Microsoft/vcpkg.git ${VCPKG_ROOT})
            # If a reproducible build is desired (and potentially old libraries are # ok), uncomment the
            # following line and pin the vcpkg repository to a specific githash.
            # execute_process(COMMAND git checkout 745a0aea597771a580d0b0f4886ea1e3a94dbca6 WORKING_DIRECTORY ${VCPKG_ROOT})
        else()
            # The following command has no effect if the vcpkg repository is in a detached head state.
            message("Auto-updating vcpkg in ${VCPKG_ROOT}")
            execute_process(COMMAND git pull WORKING_DIRECTORY ${VCPKG_ROOT})
        endif()

        if(NOT EXISTS ${VCPKG_ROOT}/README.md)
            message(FATAL_ERROR "***** FATAL ERROR: Could not clone vcpkg *****")
        endif()

        if(WIN32)
            set(BOOST_INCLUDEDIR ${VCPKG_ROOT}/installed/x86-windows/include)
            set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg.exe)
            set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.bat)
        else()
            set(VCPKG_EXEC ${VCPKG_ROOT}/vcpkg)
            set(VCPKG_BOOTSTRAP ${VCPKG_ROOT}/bootstrap-vcpkg.sh)
        endif()

        if(NOT EXISTS ${VCPKG_EXEC})
            message("Bootstrapping vcpkg in ${VCPKG_ROOT}")
            execute_process(COMMAND ${VCPKG_BOOTSTRAP} WORKING_DIRECTORY ${VCPKG_ROOT})
        endif()

        if(NOT EXISTS ${VCPKG_EXEC})
            message(FATAL_ERROR "***** FATAL ERROR: Could not bootstrap vcpkg *****")
        endif()

        set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

        #message(STATUS "***** Checking project third party dependencies in ${VCPKG_ROOT} *****")
        execute_process(
                    COMMAND ${VCPKG_EXEC} install sqlite3[core,tool] --triplet x64-windows --recurse
                    WORKING_DIRECTORY ${VCPKG_ROOT})

        message ("Setting SQlite3 MACROS")
        set (SQLite3_INCLUDE_DIR "${VCPKG_ROOT}/packages/sqlite3_x64-windows/include" )
        set (SQLite3_LIBRARY "${SQLite3_INCLUDE_DIR}/../lib/sqlite3.lib" )
        set (SQLite3_BINARY "${SQLite3_INCLUDE_DIR}/../tools/sqlite3.exe" )

    endif()

endif()


set (PROJ_FOLDER "${EXTERNAL_FOLDER}/proj-osgeo")
set (PROJ_INCLUDE_FOLDER "${PROJ_FOLDER}/include")
set (PROJ_SRC_FOLDER "${PROJ_FOLDER}/src")

set (PROJ_BUILD_DIRECTORY ${BUILD_DIRECTORY}/proj)
file (MAKE_DIRECTORY ${PROJ_BUILD_DIRECTORY})


IF (MSVC)
    set (PROJ_LIB "${PROJ_BUILD_DIRECTORY}/lib/Release/proj.lib")

    add_custom_command(
  OUTPUT "${PROJ_LIB}"
  COMMAND
        cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DBUILD_TESTING=OFF -DENABLE_CURL=OFF -DENABLE_TIFF=OFF  -DBUILD_APPS=OFF -DBUILD_PROJSYNC=OFF ${PROJ_FOLDER} &&
        cmake --build . --config Release -j 8

  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY "${PROJ_BUILD_DIRECTORY}"
)
ELSE()
    set (PROJ_LIB "${PROJ_BUILD_DIRECTORY}/lib/libproj.a")

    add_custom_command(
  OUTPUT "${PROJ_LIB}"
  COMMAND
        cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DENABLE_CURL=OFF -DENABLE_TIFF=OFF -DBUILD_APPS=OFF ${PROJ_FOLDER} &&
        cmake --build . --config Release --parallel 16
  #DEPENDS ${SOURCE_FILES} /tmp/bin/create_foo_hh main.cpp
  WORKING_DIRECTORY "${PROJ_BUILD_DIRECTORY}"
)
ENDIF()




include_directories(BEFORE ${PHREEQC_SRC_FOLDER})
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/common)
include_directories(BEFORE ${PHREEQC_SRC_FOLDER}/phreeqcpp/PhreeqcKeywords)

include_directories(BEFORE ${PROJ_INCLUDE_FOLDER})
include_directories(BEFORE ${PROJ_SRC_FOLDER})

include_directories (BEFORE ${EXTERNAL_FOLDER}/tclap/include)


if (MSVC)
    include_directories(BEFORE ${SQLite3_INCLUDE_DIR})
    include_directories(BEFORE ${EXTERNAL_FOLDER}/dirent_win)
    #include_directories(BEFORE ${EXTERNAL_FOLDER}/getopt_win)
endif()

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

list(APPEND EXTRA_LIBS ${PROJ_LIB})
list(APPEND EXTRA_LIBS ${IPHREEQC_LIB})
list(APPEND EXTRA_LIBS ${SQLite3_LIBRARY})

if (OPENMP_FOUND)
    list(APPEND EXTRA_LIBS "OpenMP::OpenMP_CXX")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS} )

# Copy DLLs to executable directory on Windows
if (MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJ_BUILD_DIRECTORY}/bin/Release/proj_9_3.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying proj_9_3.dll to executable directory"
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJ_BUILD_DIRECTORY}/bin/Release/sqlite3.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying sqlite3.dll to executable directory"
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJ_BUILD_DIRECTORY}/data/proj.db"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying proj.db to executable directory"
    )
endif()
