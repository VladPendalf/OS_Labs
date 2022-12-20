.PHONY: all run clean

all: RWLock.o
	gcc RWLock.o -std=c99 -lpthread -o RWLock

RWLock.o:	RWLock.c
	gcc -c RWLock.c -o RWLock.o

run: 
	./RWLock

clean:
	rm -f RWLock.o RWLock
