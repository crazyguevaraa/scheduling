CC=clang
CFLAGS=-c -o

all: scheduling

scheduling: main.o source.o
	$(CC) main.o source.o -o scheduling

main.o: main.c 
	$(CC) $(CFLAGS) main.c

source.o: source.c
	$(CC) $(CFLAGS) source.c

clean: 
	rm -rf *.o scheduling
