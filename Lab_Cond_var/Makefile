.PHONY: all run clean

all: Cond.o
	gcc Cond.o -std=c99 -lpthread -o Cond

Cond.o:	Cond.c
	gcc -c Cond.c -o Cond.o

run: 
	./Cond

clean:
	rm -f Cond.o Cond
