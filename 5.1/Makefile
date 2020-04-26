CC=gcc
CFLAGS=-g -Wall -O -I. -DNDEBUG
LDFLAGS=-L.
LDLIBS=-lwiringPi -pthread

all: libwiringPi.a main

main: main.o interruptor.o fsm.o timer.o kbhit.o

clean:
	$(RM) *.o *~ main libwiringPi.a

libwiringPi.a: wiringPi.o
	ar rcs $@ $^

