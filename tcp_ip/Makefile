all: client server

client : client.c 
	gcc -o $@ $^ -lpthread

server : server.c
	gcc -o $@ $^ -lpthread

clean :
	rm -f client server
