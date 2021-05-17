V ?= @

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