project(fmt)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_CPP_DIR})

set(SOURCE_FILES
        ${SGL_SRC_LIB_CPP_DIR}/fmt/format.cc
        ${SGL_SRC_LIB_CPP_DIR}/fmt/ostream.cc
        ${SGL_SRC_LIB_CPP_DIR}/fmt/posix.cc
        ${SGL_SRC_LIB_CPP_DIR}/fmt/printf.cc)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")