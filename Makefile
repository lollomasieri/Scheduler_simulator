PROJECT := simulator
CC = gcc
CFLAGS = -Wall -pedantic
DEBUG = -ggdb3 -D DEBUG
LDFLAGS = -pthread

HEADERS := $(wildcard *.h)
CFILES := $(wildcard *.c)
OBJECTS := $(wildcard *.o)


all: lib $(PROJECT)

lib: strutture.h funzioni.c funzioni.h
	$(CC) -c $(CFLAGS) $<
	
$(PROJECT): main.c
	$(CC) -c $(CFLAGS) $<
	$(CC) $(CFLAGS) $(wildcard *.o) -o $(PROJECT)
	
clean:
	@rm $(PROJECT)
	@rm $(OBJECTS)
