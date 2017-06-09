CC = gcc -Wall -Werror

all: test 

test: lqueue.c test.c
	   $(CC) -g test.c lqueue.c -o $@ 
clean:
	rm test
