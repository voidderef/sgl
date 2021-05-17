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
	$(V)echo ""

build: build-linux build-win

build-linux:
	$(V)cp -r install/linux/* $(INSTALL_LINUX_DIR)

build-win:
	$(V)cp -r install/win/* $(INSTALL_WIN_DIR)

clean: clean-import

clean-import:
	$(V)echo ""