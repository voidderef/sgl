V ?= @

OS := $(shell uname 2>/dev/null || echo Unknown)
CPUS := 1

# Mac OS X
ifeq ($(OS), Darwin)
    CPUS := $(shell sysctl -n hw.ncpu)
endif

ifeq ($(OS), Linux)
    CPUS := $(shell grep -c ^processor /proc/cpuinfo)
endif

PROJECT_DIR := $(shell pwd)

BUILD_DIR := $(PROJECT_DIR)/build
DIST_DIR := $(PROJECT_DIR)/dist
SRC_DIR := $(PROJECT_DIR)/src

BUILD_DEPS_DIR := $(shell pwd)/deps/build
BUILD_DEPS_LINUX_DIR := $(BUILD_DEPS_DIR)/linux
BUILD_DEPS_WIN_DIR := $(BUILD_DEPS_DIR)/win

BUILD_SGL_DIR := $(BUILD_DIR)/sgl
BUILD_SGL_LINUX_DIR := $(BUILD_SGL_DIR)/linux
BUILD_SGL_WIN_DIR := $(BUILD_SGL_DIR)/win

BUILD_SGL_LINUX_BIN_DIR := $(BUILD_SGL_LINUX_DIR)/bin
BUILD_SGL_WIN_BIN_DIR := $(BUILD_SGL_WIN_DIR)/bin

BUILD_PACKAGE_DIR := $(BUILD_DIR)/package
BUILD_PACKAGE_LINUX_DIR := $(BUILD_PACKAGE_DIR)/linux
BUILD_PACKAGE_WIN_DIR := $(BUILD_PACKAGE_DIR)/win

BUILD_DOCKER_DIR := $(BUILD_DIR)/docker

SRC_MAIN_LUA_SCRIPT_DIR := $(SRC_DIR)/main/lua