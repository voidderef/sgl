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
	$(V)../../script/repo-import.sh ../src/ffmpeg

build: build-linux build-win

build-linux:
	$(V)cd checkout && \
		rm -rf build && \
		mkdir build && \
		cd build && \
		../configure --prefix="$(INSTALL_LINUX_DIR)" --enable-shared && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

build-win:
	$(V)cd checkout && \
		rm -rf build && \
		mkdir build && \
		cd build && \
		../configure --prefix="$(INSTALL_WIN_DIR)" --arch=x86 --target-os=mingw32 --cross-prefix=i686-w64-mingw32- --enable-shared && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

clean: clean-import

clean-import:
	$(V)rm -rf checkout

