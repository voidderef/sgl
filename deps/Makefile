V ?= @

INSTALL_DIR := $(shell pwd)/build
INSTALL_LINUX_DIR := $(INSTALL_DIR)/linux
INSTALL_WIN_DIR := $(INSTALL_DIR)/win

SRC_DIRS := $(shell find src -mindepth 1 -maxdepth 1 -type d | cut -f 2 -d '/' | sort)

DOCKER_CONTAINER_NAME := "sgl-build-deps"
DOCKER_IMAGE_NAME := "sgl:build-deps"

all: \
usage

.PHONY: \
usage \
import \
import-% \
build \
build-output-dir \
build-% \
build-docker \
clean \
clean-% \
clean-install \
clean-docker

usage:
	$(V)echo "SGL dependency import and build makefile"
	$(V)echo "  import: Import/download source code of all (direct) dependencies for SGL"
	$(V)echo "  import-%: Import/download files/source code of a single dependency, e.g. import-zlib"
	$(V)echo "  build: Build all dependencies"
	$(V)echo "  build-%: Build a single dependency, e.g. build-zlib"
	$(V)echo "  build-docker: Build the dependencies in a docker container"
	$(V)echo "  clean: Cleanup all dependencies removing any imported data and built binaries"
	$(V)echo "  clean-%: Cleanup a single dependency removing any imported data, e.g. clean-zlib"
	$(V)echo "  clean-install: Cleanup any built output of all dependencies"
	$(V)echo "  clean-docker: Cleanup the docker build environment"
	$(V)echo ""
	$(V)echo "List of dependencies:"
	$(V)echo "$(SRC_DIRS)"

import: $(addprefix import-,$(SRC_DIRS))

import-%:
	$(V)cd src/$* && make import

# Build order to resolve dependencies
build: build-output-dir build-zlib build-libjpeg build-libpng build-freetype build-sdl2 build-sdl2-image build-sdl2-ttf build-ffmpeg

build-output-dir:
	$(V)mkdir -p $(INSTALL_LINUX_DIR)
	$(V)mkdir -p $(INSTALL_WIN_DIR)

build-%:
	$(V)cd src/$* && make build

build-docker:
	$(V)docker rm -f $(DOCKER_CONTAINER_NAME) 2> /dev/null || true
	$(V)docker build -t $(DOCKER_IMAGE_NAME) -f Dockerfile .
	$(V)docker create --name $(DOCKER_CONTAINER_NAME) $(DOCKER_IMAGE_NAME)
	$(V)mkdir -p build
	$(V)docker cp $(DOCKER_CONTAINER_NAME):/sgl/deps/build build/docker

clean: $(addprefix clean-,$(SRC_DIRS)) clean-install

clean-%:
	$(V)cd src/$* && make clean

clean-install:
	$(V)rm -rf $(INSTALL_DIR)

clean-docker:
	$(V)docker rm -f $(DOCKER_CONTAINER_NAME)
	$(V)docker image rm -f $(DOCKER_IMAGE_NAME)