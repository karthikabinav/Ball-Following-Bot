# Makefile
#

SH = bash
CFLAGS = -g
LDFLAGS = `pkg-config --cflags opencv` `pkg-config --libs opencv` -lm -lserial

all: ball

ball:working.cpp 
	gcc $(LDFLAGS) $^ -o $@


clean:
	rm -rf *.o
	rm -rf ball
	




