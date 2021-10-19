CC = gcc
CFLAGS = -static -Wall -Os
TARGET = psp-packer
OBJS = main.o
LDFLAGS= -lz

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) *.o $(TARGET) *.exe *.exe.stackdump
