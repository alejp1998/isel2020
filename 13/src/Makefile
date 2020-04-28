CC=gcc
CFLAGS=-g -Wall -O2 $(DEBUG) -Iinclude -Iinclude/freertos -Ifreertos/portable/GCC/Posix
LDFLAGS=-L.
LDLIBS=-lwiringPi -pthread -lrt

all: libwiringPi.a main

main: main.o switch.o alarm.o code.o fsm.o kbhit.o tasks.o croutine.o queue.o list.o port.o heap_3.o

clean:
	$(RM) *.o *~ main libwiringPi.a

libwiringPi.a: wiringPi.o
	ar rcs $@ $^

vpath %.c freertos
vpath %.c freertos/portable/GCC/Posix
vpath %.c freertos/portable/MemMang