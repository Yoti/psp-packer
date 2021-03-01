relase: all
	  mv complex.prx DATA.PSP


TARGET = complex
OBJS = main.o 

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
USE_USER_LIBS = 1

LIBDIR =
#LDFLAGS = -mno-crt0 -nostartfiles
LIBS += -lpsppopsman

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
