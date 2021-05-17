project(io-keyboard-sdl2)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})
include_directories(${SGL_SRC_LIB_CPP_DIR})

include_directories(${SGL_SRC_MAIN_CPP_DIR})
include_directories(${SGL_SRC_MAIN_CPP_DIR}/io/keyboard/sdl2)

set(SOURCE_FILES
		${SGL_SRC_MAIN_CPP_DIR}/io/keyboard/sdl2/io-keyboard-sdl2.cpp
        ${SGL_SRC_MAIN_CPP_DIR}/io/keyboard/sdl2/Keyboard.cpp
		${SGL_SRC_MAIN_CPP_DIR}/io/keyboard/sdl2/Mappings.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} ks)
else ()
	target_link_libraries(${PROJECT_NAME} ks dl dw)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")