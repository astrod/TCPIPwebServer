/*
 * =====================================================================================
 *
 *       Filename:  echo_stdserv.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/26/2014 07:36:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, const *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	FILE *readfp;
	FILE *writefp;
	if(argc !=2) {
		printf("Usage :%s <prot> \n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1) error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) error_handling("bind() error");
	if(listen(serv, 5)==-1) error_handling("listen() error");
	clnt_adr_sz=sizeof(clnt_adr);

	for (i = 0; i < 5; i++) {
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_sock_sz);
		if(clnt_sock == -1) error_handling("accept error");
		else printf("Connected client %d", i+1);

		readfp = fdopen(clnt_sock, "r");
		writefp = fdopen(clnt_sock, "w");
		while(!feof(readfp)) {
			fgets(message, BUF_SIZE, readfp);
			fputs(message, writefp);
			fflush(writefp);
		} //한 스트링씩 읽고 뿌려주고, 버퍼를 비우는 작업을 반복함. 데이터는readfp에서 가져옴

		fclose(readfp);
		fclose(writefp);
	}
	close(serv_sock);
	return 0;
}
	
void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}	
