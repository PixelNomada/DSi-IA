BLOCKSDS ?= /opt/wonderful/thirdparty/blocksds/core

LIBDIRS += $(BLOCKSDS)/libs/dswifi

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
