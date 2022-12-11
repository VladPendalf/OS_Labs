.PHONY: all run clean

all: Fork.o
	gcc Fork.o -std=c99 -o Fork

Fork.o:	Fork.c
	gcc -c Fork.c -o Fork.o

run: 
	./Fork

clean:
	rm -f Fork.o Fork