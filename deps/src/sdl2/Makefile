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
	$(V)../../script/repo-import.sh ../src/sdl2

build: build-linux build-win

build-linux:
	$(V)cd checkout && \
		rm -rf build && \
		./autogen.sh && \
		mkdir build && \
		cd build && \
		CPPFLAGS="-I$(INSTALL_LINUX_DIR)/include" LDFLAGS="-L$(INSTALL_LINUX_DIR)/lib" ../configure --prefix="$(INSTALL_LINUX_DIR)" --enable-video-wayland=no && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

build-win:
	$(V)cd checkout && \
		rm -rf build && \
		./autogen.sh && \
		mkdir build && \
		cd build && \
		CPPFLAGS="-I$(INSTALL_WIN_DIR)/include" LDFLAGS="-L$(INSTALL_WIN_DIR)/lib" ../configure --prefix="$(INSTALL_WIN_DIR)" --target=i686-w64-mingw32 --host=i686-w64-mingw32 --build=x86_64-linux --enable-video-opengl=yes --enable-render-d3d=no && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

clean: clean-import

clean-import:
	$(V)rm -rf checkout

