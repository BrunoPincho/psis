CC=gcc -Wall -g 
CFLAGS= -pthread
OBJ=server.o threadserv.o list.o
HEAD=threadserv.h

All: server outros

server: $(OBJ)
	$(CC) $(OBJ) -o $@ $(CFLAGS)

server.o: server.c $(HEAD)
	$(CC) -c server.c $(CFLAGS)
	

threadserv.o: threadserv.c threadserv.h 
	$(CC) -c threadserv.c $(CFLAGS)

list.o: list.c threadserv.h
	$(CC) -c list.c $(CFLAGS)

outros:
	rm *.o

clean:
	rm server
