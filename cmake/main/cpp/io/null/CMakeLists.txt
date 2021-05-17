project(io-null)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io/null)

set(SOURCE_FILES
		${SGL_SRC_MAIN_CPP_DIR}/io/null/io-null.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME})

if (WIN32)

else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif ()