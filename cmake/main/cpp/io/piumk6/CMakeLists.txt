project(io-piumk6)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io)
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io/piumk6)
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io/util)

# For KS headers to build the correct paths
add_definitions(-DKS_PLATFORM_LINUX)

# Linux, only
add_definitions(-DIO_PLATFORM_LINUX)

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/io/piumk6/PiuMk6.cpp
		${SGL_SRC_MAIN_CPP_DIR}/io/piumk6/io-piumk6.cpp
		${SGL_SRC_MAIN_CPP_DIR}/io/util/Usb.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ks dl dw usb-1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")