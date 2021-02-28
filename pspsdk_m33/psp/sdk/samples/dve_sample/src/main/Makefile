TARGET = dvesample
OBJS = main.o pspDveManager.o

INCDIR = ../include
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = ../lib
LDFLAGS = 
LIBS = -lpspkubridge -lpsprtc

PSP_FW_VERSION = 371

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = DVE Sample

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

