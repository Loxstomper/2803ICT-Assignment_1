all: client.o server.o
	cc -o client client.o 
	cc -o server server.o

client: client.o 
	cc -o client client.o common.h client.h

server: server.o 
	cc -o server server.o common.h server.h

client.o: client.c
	cc client.c -c

server.o: server.c
	cc server.c -c

clean:
	rm -rf *.o client server
