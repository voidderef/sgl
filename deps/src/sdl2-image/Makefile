include ../Common.mk

DISABLE_IMAGE_FORMATS := --enable-png=yes --enable-webp=no --enable-webp-shared=no --enable-xv=no --enable-xpm=no --enable-xcf=no --enable-pnm=no --enable-pcx=no --enable-lbm=no

usage:
	$(V)echo "Dependency build makefile"
	$(V)echo "  import: Import/download the repository/external files of the dependency"
	$(V)echo "  build: Build this dependency for Linux and Windows"
	$(V)echo "  build-linux: Build this dependency for Linux"
	$(V)echo "  build-win: Build this dependency for Windows"
	$(V)echo "  clean: Clean the build environment of this dependency"
	$(V)echo "  clean-import: Remove the imported/downloaded data"

import:
	$(V)../../script/repo-import.sh ../src/sdl2-image

build: build-linux build-win

build-linux:
	$(V)cd checkout && \
		rm -rf build && \
		./autogen.sh && \
		mkdir build && \
		cd build && \
			PKG_CONFIG_PATH="$(INSTALL_LINUX_DIR)/lib/pkgconfig" \
			CPPFLAGS="-I$(INSTALL_LINUX_DIR)/include" \
			LDFLAGS="-L$(INSTALL_LINUX_DIR)/lib" \
			../configure \
			--with-sdl-prefix="$(INSTALL_LINUX_DIR)" \
			--prefix="$(INSTALL_LINUX_DIR)" \
			$(DISABLE_IMAGE_FORMATS) && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

build-win:
	$(V)cd checkout && \
		rm -rf build && \
		./autogen.sh && \
		mkdir build && \
		cd build && \
			PKG_CONFIG_PATH="$(INSTALL_WIN_DIR)/lib/pkgconfig" \
			CPPFLAGS="-I$(INSTALL_WIN_DIR)/include" \
			LDFLAGS="-L$(INSTALL_WIN_DIR)/lib" \
			../configure \
			--with-sdl-prefix="$(INSTALL_WIN_DIR)" \
			--prefix="$(INSTALL_WIN_DIR)" \
			$(DISABLE_IMAGE_FORMATS) \
			--target=i686-w64-mingw32 \
			--host=i686-w64-mingw32 \
			--build=x86_64-linux && \
		make -j $(CPUS) install && \
		cd .. && \
		rm -rf build

clean: clean-import

clean-import:
	$(V)rm -rf checkout