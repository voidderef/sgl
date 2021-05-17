project(io-iidxio)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_C_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io)
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io/iidxio)

# For KS headers to build the correct paths
add_definitions(-DKS_PLATFORM_WIN32)

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/io/bt/BemanitoolsGlue.cpp
		${SGL_SRC_MAIN_CPP_DIR}/io/iidxio/IIDXIO.cpp
		${SGL_SRC_MAIN_CPP_DIR}/io/iidxio/io-iidxio.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Win32, only
# Link stuff staticly
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static -static-libstdc++")

target_link_libraries(${PROJECT_NAME} fmt ks bt-iidxio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")