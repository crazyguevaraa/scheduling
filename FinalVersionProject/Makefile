CC=gcc
CFLAGS=-c -o

all: scheduling

scheduling: main.o list.o task.o print.o sort.o alloctable.o processor.o
		$(CC) main.o list.o task.o print.o sort.o alloctable.o processor.o -o scheduling

main.o: main.c 
		$(CC) $(CFLAGS) main.o main.c	

list.o: list.c 
		$(CC) $(CFLAGS) list.o list.c

task.o: task.c 
		$(CC) $(CFLAGS) task.o task.c 

print.o: print.c 	
		$(CC) $(CFLAGS) print.o print.c 	

sort.o: sort.c 
		$(CC) $(CFLAGS) sort.o sort.c 

alloctable.o: alloctable.c 
		$(CC) $(CFLAGS) alloctable.o alloctable.c 

processor.o: processor.c 
		$(CC) $(CFLAGS) processor.o processor.c 

clean:
		rm -rf *.o scheduling

