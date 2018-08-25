all: client.o server.o
	cc -o client client.o 
	cc -o server server.o

client: client.o 
	cc -o client client.o common.h client.h

server: server.o server_func.o
	cc -o server server.o server_func.o common.h server_func.h

client.o: client.c
	cc client.c -c

server.o: server.c 
	cc server.c -c

server_func.o: server_func.c
	cc server_func.c -c

clean:
	rm -rf *.o client server
