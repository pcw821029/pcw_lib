/*****************************************
 *	파일 이름 : 
 *	작성자 : 박철우
 *	작성일 : 2017.06.09
 *	e-mail : pcw4844@naver.com
 *	설 명 : 간단한 TCP/IP 프로토콜을 이용
 *	하여 클라이언트와 메시지를 주고 받는
 *	역할을 하는 프로그램
 * **************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>

#define BUFF_SIZE 100 
#define CLIENT_MAX 256 

void error_handling(char *message);
void *client_handler(void *arg);
void send_msg(void *msg, int len);

int count_client = 0;
int socks_client[CLIENT_MAX];
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
	int sock_server;
	int sock_client;

	struct sockaddr_in addr_server;
	struct sockaddr_in addr_client;
	socklen_t size_addr_client;

	pthread_t pthread_id;	

	if(argc!=2){ 
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
										
	pthread_mutex_init(&mutex, NULL);
	sock_server = socket(PF_INET, SOCK_STREAM, 0);
	
	if(-1 == sock_server)
		error_handling("socket() error.");

	memset(&addr_server, 0, sizeof(addr_server));
	addr_server.sin_family = AF_INET;
	addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_server.sin_port = htons(atoi(argv[1]));

	if(bind( sock_server, (struct sockaddr*)&addr_server,sizeof(addr_server))==-1)
		error_handling("bind() error");

	if(listen(sock_server, 5)==-1)
		error_handling("listen() error");

	while(1){
		size_addr_client = sizeof(addr_client);
		sock_client = accept( sock_server, (struct sockaddr*)&addr_client, &size_addr_client);

		pthread_mutex_lock(&mutex);
		socks_client[count_client++] = sock_client;
		pthread_mutex_unlock(&mutex);

		pthread_create(&pthread_id, NULL, client_handler, (void *)&sock_client);
		pthread_detach(pthread_id);
		printf("Client IP:%s \n", inet_ntoa(addr_client.sin_addr));

	}//end of while 

	close(sock_server);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(EXIT_FAILURE);
}

//각 클라이언트를 관리하는 함수
void *client_handler(void *arg)
{
	int sock_client = *((int *)arg);
	int str_len = 0, i;
	void* msg;
	char* _msg;

	msg=(char*)malloc(BUFF_SIZE);
	_msg = (char*)msg;
	while((str_len = read(sock_client, msg, BUFF_SIZE-1)) > 0){
		_msg[str_len] = 0;
		printf("Recv msg : %s", _msg);
		send_msg(msg, str_len);
	}
//클라이언트의 접속이 끊기게 되면 수행되며, 
//클라이언트 리스트를 갱신하기 위함
	pthread_mutex_lock(&mutex);
	for(i=0; i<count_client; i++){
		if(sock_client == socks_client[i]){
			while(i++ <= count_client-1){
				socks_client[i] = socks_client[i+1];
			}
			break;
		}
	}
	count_client--;
	pthread_mutex_unlock(&mutex);
	close(sock_client);

	return NULL;
}

//클라이언트에게 메시지 보내는 함수
void send_msg(void *msg, int len)
{
	int i;
	pthread_mutex_lock(&mutex);
	for(i=0; i<count_client; i++){
		write(socks_client[i], msg, len);
	}
	pthread_mutex_unlock(&mutex);
}
