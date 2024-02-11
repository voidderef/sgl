#
# Packiging build rules
#

include Common.mk

PACKAGE_LINUX_ZIP := "sgl-linux.zip"
PACKAGE_LINUX_EXAMPLE_ZIP := "sgl-linux-example.zip"
PACKAGE_WIN_ZIP := "sgl-win.zip"
PACKAGE_WIN_EXAMPLE_ZIP := "sgl-win-example.zip"

BUILD_PACKAGE_LINUX_EXAMPLE_DIR := $(BUILD_PACKAGE_LINUX_DIR)-example
BUILD_PACKAGE_WIN_EXAMPLE_DIR := $(BUILD_PACKAGE_WIN_DIR)-example

DIST_DIR_DATA_EXAMPLE := $(DIST_DIR)/data/example

$(BUILD_PACKAGE_LINUX_DIR):
	$(V)mkdir -p $@
	$(V)mkdir -p $@/data
	$(V)mkdir -p $@/data/io
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-joystick-sdl2.so $@/data/io/libio-joystick-sdl2.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-keyboard-sdl2.so $@/data/io/libio-keyboard-sdl2.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-null.so $@/data/io/libio-null.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-piubtn.so $@/data/io/libio-piubtn.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-piumk6.so $@/data/io/libio-piumk6.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-piulxio.so $@/data/io/libio-piulxio.so
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libio-piumk6itg.so $@/data/io/libio-piumk6itg.so
	$(V)cp -r $(SRC_MAIN_LUA_SCRIPT_DIR) $@/data
	$(V)mkdir -p $@/modules
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/libks.so $@/modules/libks.so
	$(V)cp $(BUILD_DEPS_LINUX_DIR)/lib/*.so* $@/modules
	$(V)mkdir -p $@/save
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/io-test $@/io-test
	$(V)cp $(BUILD_SGL_LINUX_BIN_DIR)/sgl $@/sgl
	$(V)cp $(DIST_DIR)/sgl.sh $@/sgl.sh
	$(V)cp $(DIST_DIR)/sgl-trap.sh $@/sgl-trap.sh

$(BUILD_PACKAGE_WIN_DIR):
	$(V)mkdir -p $@
	$(V)mkdir -p $@/data
	$(V)mkdir -p $@/data/io
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libbt-iidxio.dll $@/data/io/libbt-iidxio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libbt-jbio.dll $@/data/io/libbt-jbio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libbt-ddrio.dll $@/data/io/libbt-ddrio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-iidxio.dll $@/data/io/libio-iidxio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-jbio.dll $@/data/io/libio-jbio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-ddrio.dll $@/data/io/libio-ddrio.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-joystick-sdl2.dll $@/data/io/libio-joystick-sdl2.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-keyboard-sdl2.dll $@/data/io/libio-keyboard-sdl2.dll
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libio-null.dll $@/data/io/libio-null.dll
	$(V)cp -r $(SRC_MAIN_LUA_SCRIPT_DIR) $@/data
	$(V)mkdir -p $@/modules
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/libks.dll $@/modules/libks.dll
	$(V)cp $(BUILD_DEPS_WIN_DIR)/bin/*.dll $@/modules
	$(V)mv $@/modules/libz.dll $@/modules/libzlib.dll
	$(V)mkdir -p $@/save
	$(V)cp $(BUILD_SGL_WIN_BIN_DIR)/sgl.exe $@/sgl.exe
	$(V)cp $(DIST_DIR)/sgl.bat $@/sgl.bat

$(BUILD_PACKAGE_LINUX_EXAMPLE_DIR): $(BUILD_PACKAGE_LINUX_DIR)
	$(V)cp -r $^ $@
	$(V)cp -r $(DIST_DIR_DATA_EXAMPLE)/* $@/data

$(BUILD_PACKAGE_WIN_EXAMPLE_DIR): $(BUILD_PACKAGE_WIN_DIR)
	$(V)cp -r $^ $@
	$(V)cp -r $(DIST_DIR_DATA_EXAMPLE)/* $@/data

$(BUILD_PACKAGE_DIR)/$(PACKAGE_LINUX_ZIP): $(BUILD_PACKAGE_LINUX_DIR)
	$(V)echo ... $@
	$(V)cd $^ && zip -r $@ .

$(BUILD_PACKAGE_DIR)/$(PACKAGE_LINUX_EXAMPLE_ZIP): $(BUILD_PACKAGE_LINUX_EXAMPLE_DIR)
	$(V)echo ... $@
	$(V)cd $^ && zip -r $@ .

$(BUILD_PACKAGE_DIR)/$(PACKAGE_WIN_ZIP): $(BUILD_PACKAGE_WIN_DIR)
	$(V)echo ... $@
	$(V)cd $^ && zip -r $@ .

$(BUILD_PACKAGE_DIR)/$(PACKAGE_WIN_EXAMPLE_ZIP): $(BUILD_PACKAGE_WIN_EXAMPLE_DIR)
	$(V)echo ... $@
	$(V)cd $^ && zip -r $@ .

package: package-linux package-win

package-linux: \
		$(BUILD_PACKAGE_LINUX_DIR) \
		$(BUILD_PACKAGE_DIR)/$(PACKAGE_LINUX_ZIP) \
		$(BUILD_PACKAGE_LINUX_EXAMPLE_DIR) \
		$(BUILD_PACKAGE_DIR)/$(PACKAGE_LINUX_EXAMPLE_ZIP)

package-win: \
		$(BUILD_PACKAGE_WIN_DIR) \
		$(BUILD_PACKAGE_DIR)/$(PACKAGE_WIN_ZIP) \
		$(BUILD_PACKAGE_WIN_EXAMPLE_DIR) \
		$(BUILD_PACKAGE_DIR)/$(PACKAGE_WIN_EXAMPLE_ZIP)

clean-package: \
		clean-package-linux \
		clean-package-win

clean-package-linux:
	$(V)rm -rf $(BUILD_PACKAGE_LINUX_DIR)
	$(V)rm -rf $(BUILD_PACKAGE_DIR)/$(PACKAGE_LINUX_ZIP)

clean-package-win:
	$(V)rm -rf $(BUILD_PACKAGE_WIN_DIR)
	$(V)rm -rf $(BUILD_PACKAGE_DIR)/$(PACKAGE_WIN_ZIP)
