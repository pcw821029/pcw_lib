/*****************************************
 *	파일 이름 : client.c
 *	작성자 : 박철우
 *	작성일 : 2017.06.09
 *	e-mail : pcw4844@naver.com
 *	설 명 : 간단한 TCP/IP 프로토콜을 이용
 *	하여 서버와 메시지를 주고 받는 역할을 
 *	하는 프로그램
 * **************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUFF_SIZE 100 
#define NAME_SIZE 20 

void error_handling(char *message);
void *send_msg(void *msg);
void *recv_msg(void *msg);

//Global data section
char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUFF_SIZE];

int main(int argc, char *argv[])
{
	int sock_server;
	struct sockaddr_in addr_server;
	pthread_t pthread_id_send, pthread_id_recv;	
	void *thread_ret;

	if(argc!=4){ 
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}
	
	sprintf(name, "[%s]", argv[3]);
	sock_server = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == sock_server)
		error_handling("socket() error.");

	memset(&addr_server, 0, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = inet_addr(argv[1]);
	addr_server.sin_port = htons(atoi(argv[2]));
	if(-1 == connect(sock_server, (struct sockaddr*)&addr_server, sizeof(addr_server)))
		error_handling("Error connect()");

	pthread_create(&pthread_id_send, NULL, send_msg, (void*)&sock_server);
	pthread_create(&pthread_id_recv, NULL, recv_msg, (void*)&sock_server);
	pthread_join(pthread_id_send, &thread_ret);
	pthread_join(pthread_id_recv, &thread_ret);

	fputs("End of Client!\n", stderr);
	close(sock_server);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

void *send_msg(void *arg)
{
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE+BUFF_SIZE];
	while(1){
		printf("Input Message(Q quit):");
		fgets(msg, BUFF_SIZE, stdin);
		if( !strcmp(msg, "q\n") || !strcmp(msg, "Q\n") ){
			fputs("Quit!\n", stderr);
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
		fputs("send msg!\n", stderr);
	}
	return NULL;
}

void *recv_msg(void *arg)
{
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE+BUFF_SIZE];
	int str_len;
	while(1){
		str_len = read(sock, name_msg, NAME_SIZE+BUFF_SIZE-1);
		if(str_len < 0) 
			return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
	return NULL;
}
