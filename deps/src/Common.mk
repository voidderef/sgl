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

INSTALL_DIR := $(shell pwd)/../../build
INSTALL_LINUX_DIR := $(INSTALL_DIR)/linux
INSTALL_WIN_DIR := $(INSTALL_DIR)/win

all: \
usage

.PHONY: \
usage \
build \
build-linux \
build-win \
clean \
clean-import