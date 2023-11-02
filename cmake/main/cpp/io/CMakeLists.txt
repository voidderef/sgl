project(io)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io)

# For KS headers to build the correct paths
if (WIN32)
    add_definitions(-DKS_PLATFORM_WIN32)
else ()
    add_definitions(-DKS_PLATFORM_LINUX)
endif ()

if (WIN32)
    add_definitions(-DIO_PLATFORM_WIN32)
else ()
    add_definitions(-DIO_PLATFORM_LINUX)
endif ()

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/io/IOManager.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/io/IOStateProcessorManager.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ks)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
else ()
    target_link_libraries(${PROJECT_NAME} ks)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

add_subdirectory(null)

if (WIN32)
    add_subdirectory(ddrio)
    add_subdirectory(iidxio)
    add_subdirectory(jbio)
else ()
    add_subdirectory(test)
    add_subdirectory(piubtn)
    add_subdirectory(piumk6)
    add_subdirectory(piumk6itg)
endif ()

add_subdirectory(joystick)
add_subdirectory(keyboard)