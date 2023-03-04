include Common.mk

DOCKER_CONTAINER_NAME := "sgl-build"
DOCKER_IMAGE_NAME := "sgl:build"

all: \
usage

.PHONY: \
usage \
build-deps \
build \
build-linux \
build-win \
clean \
clean-build \
clean-build-linux \
clean-build-win \
build-docker-deps \
build-docker \
clean-docker \
package \
package-linux \
package-win \
clean-package \
clean-package-linux \
clean-package-win

usage:
	$(V)echo "SGL project build makefile"
	$(V)echo "  build-deps: Build the dependencies for SGL (see deps subfolder)"
	$(V)echo "  build: Build SGL, requires running the build-deps target to have all dependencies built"
	$(V)echo "  build-linux: Same as build target but Linux platform, only"
	$(V)echo "  build-win: Same as build target but Windows platform, only"
	$(V)echo "  clean: Clean the build and package output"
	$(V)echo "  clean-build: Clean build output, only"
	$(V)echo "  clean-build-linux: Clean Linux build output, only"
	$(V)echo "  clean-build-win: Clean Windows build output, only"
	$(V)echo "  build-docker-deps: Build the dependencies in a docker container"
	$(V)echo "  build-docker: Build SGL in a docker container, requires running the build-docker-deps target"
	$(V)echo "  build-docker-release: Build a release in docker. Full clean, deps docker build and sgl docker build"
	$(V)echo "  clean-docker: Delete the docker container used for building SGL"
	$(V)echo "  package: Package the build output for distribution"
	$(V)echo "  package-linux: Same as package target but Linux, only"
	$(V)echo "  package-win: Same as package target but Windows, only"
	$(V)echo "  clean-package: Cleanup package output"
	$(V)echo "  clean-package-linux: Cleanup package output, Linux only"
	$(V)echo "  clean-package-win: Cleanup package output, Windows only"

build-deps:
	$(V)cd deps && \
		make import && \
		make build

build: build-linux build-win

build-linux:
	$(V)mkdir -p $(BUILD_SGL_LINUX_DIR)
	$(V)cd $(BUILD_SGL_LINUX_DIR) && \
		cmake \
			-DCMAKE_CXX_FLAGS="-I$(BUILD_DEPS_LINUX_DIR)/include" \
			-DCMAKE_CXX_LINK_FLAGS="-L$(BUILD_DEPS_LINUX_DIR)/lib" \
			../../.. && \
		make -j $(CPUS)

build-win:
	$(V)mkdir -p $(BUILD_SGL_WIN_DIR)
	$(V)cd $(BUILD_SGL_WIN_DIR) && \
		cmake \
			-DCMAKE_TOOLCHAIN_FILE=../../../cmake/Toolchain-mingw32.cmake \
			-DCMAKE_CXX_FLAGS="-I$(BUILD_DEPS_WIN_DIR)/include" \
			-DCMAKE_CXX_LINK_FLAGS="-L$(BUILD_DEPS_WIN_DIR)/lib" \
			../../.. && \
		make -j $(CPUS)

# Don't clean dependencies as these take a long time to build and usually don't need to be re-built for a clean build
# of SGL
clean: clean-build clean-package

clean-build: clean-build-linux clean-build-win

clean-build-linux:
	$(V)rm -rf $(BUILD_SGL_LINUX_DIR)

clean-build-win:
	$(V)rm -rf $(BUILD_SGL_WIN_DIR)

build-docker-deps:
	$(V)cd deps && make build-docker

build-docker:
	$(V)docker rm -f $(DOCKER_CONTAINER_NAME) 2> /dev/null || true
	$(V)docker build -t $(DOCKER_IMAGE_NAME) -f Dockerfile .
	$(V)docker create --name $(DOCKER_CONTAINER_NAME) $(DOCKER_IMAGE_NAME)
	$(V)rm -rf $(BUILD_DOCKER_DIR)
	$(V)mkdir -p $(BUILD_DOCKER_DIR)
	$(V)docker cp $(DOCKER_CONTAINER_NAME):/sgl/build/package $(BUILD_DOCKER_DIR)/package

clean-docker:
	$(V)docker rm -f $(DOCKER_CONTAINER_NAME) || true
	$(V)docker image rm -f $(DOCKER_IMAGE_NAME) || true
	$(V)rm -rf $(BUILD_DOCKER_DIR)

build-docker-release: clean clean-docker build-docker-deps build-docker

include Package.mk