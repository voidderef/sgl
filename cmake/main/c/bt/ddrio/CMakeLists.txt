project(bt-ddrio)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_MAIN_C_DIR}/bt/api)
include_directories(${SGL_SRC_MAIN_C_DIR}/bt/ddrio)

set(SOURCE_FILES
        ${SGL_SRC_MAIN_C_DIR}/bt/ddrio/ddrio-stub.c)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Win32, only
# Link stuff staticly
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static -static-libstdc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")