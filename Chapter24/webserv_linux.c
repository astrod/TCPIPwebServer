/*
 * =====================================================================================
 *
 *       Filename:  webserv_linux.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/28/2014 04:22:25 AM
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
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_data(FILE *fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char* message);

int main(int argc, const char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_size;
	char buf[BUF_SIZE];
	pthread_t t_id;
	if(argc !=2) {
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))== -1) error_handling("bind()error!");
	if(listen(serv_sock, 20)==-1) error_handling("listen error!");

	while(1) {
		clnt_adr_size=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
		printf("connected Request : %s : %d \n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		pthread_detach(t_id);
	}
	close(serv_sock);
	return 0;
}

void* request_handler(void *arg) {
	int clnt_sock=*((int*)arg);
	char req_line[SMALL_BUF];
	FILE* clnt_read;
	FILE* clnt_write;

	char method[10];
	char ct[15];
	char file_name[30];

	clnt_read=fdopen(clnt_sock, "r");
	clnt_write=fdopen(dup(clnt_sock), "w");
	fgets(req_line, SMALL_BUF, clnt_read);
	if(strstr(req_line, "HTTP/")==NULL) {
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return;
	}
	
//	printf("%s", req_line);

	strcpy(method, strtok(req_line, " /")); //strcpy는 문자열을 자르는 함수이다. 이 함수는 / 앞에 문자열을 잘라서 그 자른 문단의 첫 번째 글자의 포인터를 리턴한다. 호출할 때마다 다음 자른 문자의 가장 앞 글자의 포인터를 리턴하는데, 이 경우에는 호출 어규먼트가 strtok(NULL "/")이 된다. 
	strcpy(file_name, strtok(NULL, " /"));
	strcpy(ct, content_type(file_name));
/*	if(strcmp(method, "GET") != 0) {
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return;
	}*/
	printf("%s\n", ct);
	fclose(clnt_read);
	printf("%s\n", file_name);
	send_data(clnt_write, ct, file_name);
}

void send_data(FILE* fp, char* ct, char* file_name) {
	char protocol[]="HTTP/1.0 200 OK\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[]="Content-length : 2048\r\n";
	char cnt_type[SMALL_BUF];
	char buf[BUF_SIZE];
	FILE* send_file;
	char *testPointer;

	printf("%s \n", file_name);
printf("call send data \n");

	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
	printf("two stage \n");
	send_file=fopen(file_name, "r");
	printf("can i send_file? \n");
	printf("%s\n", file_name);
	if(send_file==NULL) {
		send_error(fp);
		return;
	}

printf("reach  data \n");

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);

	while(fgets(buf, BUF_SIZE, send_file)!=NULL) 
	{	
		fputs(buf, fp);
		fflush(fp);
	}
	fflush(fp);
	fclose(fp);
}

char* content_type(char* file) {
	char extension[SMALL_BUF];
	char file_name[SMALL_BUF];
	strcpy(file_name, file);
	//printf("%s \n", file_name);
	strtok(file_name, ".");
	strcpy(extension, strtok(NULL, "."));
	//printf("%s \n", extension);
	if(!strcmp(extension, "html")||!strcmp(extension, "htm")) return "text/html";
	else return "text/plain";
}

void send_error(FILE*fp) {
	char protocol[]="HTTP/1.0 400 Bad Request \r\n";
	char server[]="Server :Linux Web Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[]="Content-type :text/html\r\n\r\n";
	char content[] ="<html><head><title>NETWORK</title></head>"
		"<body><font size=+5><br>error occur! check your file name and request method"
		"</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fflush(fp);
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
