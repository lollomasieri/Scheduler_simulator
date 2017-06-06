PROJECT := simulator
CC = gcc
CFLAGS = -Wall -pedantic
DEBUG = -ggdb3 -D DEBUG
LDFLAGS = -pthread

HEADERS := $(wildcard *.h)
CFILES := $(wildcard *.c)
OBJECTS := $(wildcard *.o)


all: lib func scheduler $(PROJECT)

lib: strutture.h 
	$(CC) -c $(CFLAGS) $< 

func: funzioni.c funzioni.h
	$(CC) -c $(CFLAGS) $<

scheduler: scheduler.c scheduler.h 
	$(CC) -c $(CFLAGS) $<

$(PROJECT): main.c
	$(CC) -c $(CFLAGS) $<
	$(CC) $(CFLAGS) $(LDFLAGS) $(wildcard *.o) -o $(PROJECT)
	
clean:
	@rm $(PROJECT)
	@rm $(OBJECTS)
