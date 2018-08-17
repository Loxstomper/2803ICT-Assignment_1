all: client.o server.o
	cc client.o -o client
	cc server.o -o server

client: client.o
	cc client.o -o client

server: server.o
	cc server.o -o server

client.o: client.c
	cc -c client.c

server.o: server.c
	cc -c server.c

clean:
	rm -rf *.o
	rm -rf client
	rm -rf server

