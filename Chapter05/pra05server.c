#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024;

void error_handling(char *message);
char* moveArray(char* message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int i, msgSize;
	int str_len;
	char message[BUF_SIZE];

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;


	if(argc!=2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock =socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");


	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error!");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept() error!");
	for(i=0; i<3; i++) {
		fputs("type your word : ", stdout);
		scanf("%s", message);
		msgSize = sizeof(message);
		message = moveArray(message);
		(int)message[4] = msgSize;
		write(clnt_sock, message, sizeof(message));
		str_len = read(clnt_sock, message, BUF_SIZE-1);
		message[str_len] = 0;
		printf("message from server : %s\n", message);
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

char* moveArray(char* message) {
	int i, msgSize;
	msgSize = sizeof(message);
	for(i=0; i<msgSize; i++) {
		message[msgSize-i+3] = message[msgSize-i-1];
	}

	return message;
}