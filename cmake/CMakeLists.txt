cmake_minimum_required(VERSION 2.8)

set(SGL_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

set(CMAKE_CXX_STANDARD 14)

# force out of source build
message(STATUS "Force out of source build check...")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)

if (insource)
    message(FATAL_ERROR "Do not build in your source dir. Use a dedicated build folder. Now go cleanup the cache in the CMakeCache.txt and CMakeFiles folder which don't belong there!")
endif ()

# set git revision
execute_process(COMMAND git log -1 --format=%h --date=short HEAD WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_VARIABLE GITREV ERROR_QUIET)
string(STRIP "${GITREV}" GITREV)
if (GITREV STREQUAL "")
    set(GITREV "not available")
endif ()

# binary output dir
set(SGL_BIN_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE STRING "Output path for compiled binaries")

# For generic no-config case
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SGL_BIN_OUTPUT_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SGL_BIN_OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SGL_BIN_OUTPUT_PATH})

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INSTALL_RPATH "/sgl/deps/build/linux/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGITREV=${GITREV} -g")

if (WIN32)
	# Force win xp toolchain
    set(CMAKE_GENERATOR_TOOLSET "v140_xp" CACHE STRING "Select the windows toolchain for compilation")
endif ()

# Path variables
set(SGL_SRC_DIR "${SGL_ROOT_DIR}/src")
set(SGL_SRC_LIB_DIR "${SGL_SRC_DIR}/lib")
set(SGL_SRC_LIB_C_DIR "${SGL_SRC_LIB_DIR}/c")
set(SGL_SRC_LIB_CPP_DIR "${SGL_SRC_LIB_DIR}/cpp")
set(SGL_SRC_MAIN_DIR "${SGL_SRC_DIR}/main")
set(SGL_SRC_MAIN_C_DIR "${SGL_SRC_MAIN_DIR}/c")
set(SGL_SRC_MAIN_CPP_DIR "${SGL_SRC_MAIN_DIR}/cpp")

# Project environments
add_subdirectory(lib)
add_subdirectory(main)
