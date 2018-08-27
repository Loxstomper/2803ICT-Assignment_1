all: client.o server.o client_func.o server_func.o common.o
	cc -o client client.o client_func.o common.o common.h client.h
	cc -o server server.o server_func.o common.o common.h server_func.h

client: client.o  client_func.o common.o
	cc -o client client.o client_func.o common.o common.h client.h

server: server.o server_func.o common.o
	cc -o server server.o server_func.o common.o common.h server_func.h

common.o: common.c
	cc common.c -c

client.o: client.c 
	cc client.c -c

client_func.o: client_func.c
	cc client_func.c -c

server.o: server.c 
	cc server.c -c

server_func.o: server_func.c
	cc server_func.c -c

clean:
	rm -rf *.o client server
