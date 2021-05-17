include ../Common.mk

usage:
	$(V)echo "Dependency build makefile"
	$(V)echo "  import: Import/download the repository/external files of the dependency"
	$(V)echo "  build: Build this dependency for Linux and Windows"
	$(V)echo "  build-linux: Build this dependency for Linux"
	$(V)echo "  build-win: Build this dependency for Windows"
	$(V)echo "  clean: Clean the build environment of this dependency"
	$(V)echo "  clean-import: Remove the imported/downloaded data"

import:
	$(V)../../script/repo-import.sh ../src/zlib

build: build-linux build-win

build-linux:
	$(V)cd checkout && \
		rm -rf build && \
		mkdir build && \
		cd build && \
		cmake -G"Unix Makefiles" -DCMAKE_MAKE_PROGRAM=make  -DCMAKE_INSTALL_PREFIX="$(INSTALL_LINUX_DIR)" .. && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

build-win:
	$(V)cd checkout && \
		rm -rf build && \
		mkdir build && \
		cd build && \
		printf "set(CMAKE_SYSTEM_NAME Windows)\nset(CMAKE_SYSTEM_PROCESSOR X86)\nset(CMAKE_C_COMPILER i686-w64-mingw32-gcc)\nset(CMAKE_RC_COMPILER i686-w64-mingw32-windres)" > toolchain.cmake && \
		cmake -G"Unix Makefiles" -DCMAKE_MAKE_PROGRAM=make  -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -DCMAKE_INSTALL_PREFIX="$(INSTALL_WIN_DIR)" .. && \
		make -j $(CPUS) install && \
		mv "$(INSTALL_WIN_DIR)/lib/libzlib.dll.a" "$(INSTALL_WIN_DIR)/lib/libz.dll.a" && \
		mv "$(INSTALL_WIN_DIR)/lib/libzlibstatic.a" "$(INSTALL_WIN_DIR)/lib/libzstatic.a" && \
		mv "$(INSTALL_WIN_DIR)/bin/libzlib.dll" "$(INSTALL_WIN_DIR)/bin/libz.dll" && \
		cd .. && \
		rm -rf build

clean: clean-import

clean-import:
	$(V)rm -rf checkout

