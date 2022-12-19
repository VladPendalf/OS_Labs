.PHONY: all run clean

all: Mutex.o
	gcc Mutex.o -std=c99 -o Mutex

Mutex.o:	Mutex.c
	gcc -c Mutex.c -o Mutex.o

run: 
	./Mutex

clean:
	rm -f Mutex.o Mutex
