client: client.o
	cc -o client client.o

server: server.o
	cc -o server server.o 

client.o: client.c
	cc client.c -c

server.o: server.c
	cc server.c -c

clean:
	rm -rf *.o client server
