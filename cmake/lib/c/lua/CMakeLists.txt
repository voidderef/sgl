project(lua)
message(STATUS "Project " ${PROJECT_NAME})

include_directories(${SGL_SRC_LIB_C_DIR})

set(SOURCE_FILES
        ${SGL_SRC_LIB_C_DIR}/lua/lapi.c
        ${SGL_SRC_LIB_C_DIR}/lua/lauxlib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lbaselib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lbitlib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lcode.c
        ${SGL_SRC_LIB_C_DIR}/lua/lcorolib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lctype.c
        ${SGL_SRC_LIB_C_DIR}/lua/ldblib.c
        ${SGL_SRC_LIB_C_DIR}/lua/ldebug.c
        ${SGL_SRC_LIB_C_DIR}/lua/ldo.c
        ${SGL_SRC_LIB_C_DIR}/lua/ldump.c
        ${SGL_SRC_LIB_C_DIR}/lua/lfunc.c
        ${SGL_SRC_LIB_C_DIR}/lua/lgc.c
        ${SGL_SRC_LIB_C_DIR}/lua/linit.c
        ${SGL_SRC_LIB_C_DIR}/lua/liolib.c
        ${SGL_SRC_LIB_C_DIR}/lua/llex.c
        ${SGL_SRC_LIB_C_DIR}/lua/lmathlib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lmem.c
        ${SGL_SRC_LIB_C_DIR}/lua/loadlib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lobject.c
        ${SGL_SRC_LIB_C_DIR}/lua/lopcodes.c
        ${SGL_SRC_LIB_C_DIR}/lua/loslib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lparser.c
        ${SGL_SRC_LIB_C_DIR}/lua/lstate.c
        ${SGL_SRC_LIB_C_DIR}/lua/lstring.c
        ${SGL_SRC_LIB_C_DIR}/lua/lstrlib.c
        ${SGL_SRC_LIB_C_DIR}/lua/ltable.c
        ${SGL_SRC_LIB_C_DIR}/lua/ltablib.c
        ${SGL_SRC_LIB_C_DIR}/lua/ltm.c
        ${SGL_SRC_LIB_C_DIR}/lua/lua.c
        ${SGL_SRC_LIB_C_DIR}/lua/lundump.c
        ${SGL_SRC_LIB_C_DIR}/lua/lutf8lib.c
        ${SGL_SRC_LIB_C_DIR}/lua/lvm.c
        ${SGL_SRC_LIB_C_DIR}/lua/lzio.c)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")