PSPSDK=$(shell psp-config --pspsdk-path)
scePopsMan_OBJS = scePopsMan_0000.o scePopsMan_0001.o scePopsMan_0002.o scePopsMan_0003.o scePopsMan_0004.o scePopsMan_0005.o scePopsMan_0006.o scePopsMan_0007.o scePopsMan_0008.o scePopsMan_0009.o scePopsMan_0010.o scePopsMan_0011.o scePopsMan_0012.o scePopsMan_0013.o scePopsMan_0014.o scePopsMan_0015.o scePopsMan_0016.o 


%.o: scePopsMan.S
	psp-gcc -g -O2 -G0 -Wall -I${PSPSDK}/include -DF_$* $< -c -o $@

all: $(scePopsMan_OBJS)
	psp-ar cru libpsppopsman.a $(scePopsMan_OBJS)
	psp-ranlib libpsppopsman.a

clean:
	rm -f *.o
	rm -f *.a
