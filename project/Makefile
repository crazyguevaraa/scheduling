CC=clang
CFLAGS=-c -o

all: scheduling

scheduling: main.o source2.o
	$(CC) main.o source2.o -o scheduling

main.o: main.c 
	$(CC) $(CFLAGS) main.c

source2.o: source.c
	$(CC) $(CFLAGS) source.c

clean: 
	rm -rf *.o scheduling
