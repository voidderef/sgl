project(io-test)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io)

if (WIN32)
    add_definitions(-DKS_PLATFORM_WIN32)
else ()
    add_definitions(-DKS_PLATFORM_LINUX)
endif ()

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/io/test/IOSPTestNull.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/io/test/IOTest.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ks io)
else ()
    target_link_libraries(${PROJECT_NAME} ks io pthread)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()