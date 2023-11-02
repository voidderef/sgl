project(ks)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR}/ks)

if (WIN32)
    add_definitions(-DKS_PLATFORM_WIN32)
else ()
    add_definitions(-DKS_PLATFORM_LINUX)
endif ()

set(SOURCE_FILES
        ${SGL_SRC_MAIN_CPP_DIR}/ks/DynamicLibrary.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/FileMemory.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/FileStream.cpp
		${SGL_SRC_MAIN_CPP_DIR}/ks/FileSystem.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/Logger.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/Process.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/Random.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/Settings.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/SettingsCmdArgsLoader.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/StringUtils.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/System.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/ks/Trace.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if (WIN32)
	# Link stuff staticly
    # Note: This doesn't work anymore if we move commonly shared stuff to
    # a kitchen sink lib
	# set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static -static-libstdc++")

    target_link_libraries(${PROJECT_NAME} fmt)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
else ()
    target_link_libraries(${PROJECT_NAME} fmt dl dw)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
endif ()