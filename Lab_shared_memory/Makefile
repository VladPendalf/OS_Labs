.PHONY: all run clean

all: file1.o file2.o
	gcc file1.o -std=c99 -o file1
	gcc file2.o -std=c99 -o file2

file1.o: file1.c
	gcc -c file1.c -o file1.o

file2.o: file2.c
	gcc -c file2.c -o file2.o

run: 
	./file1 file2

clean:
	rm -f *.o file1 file2
