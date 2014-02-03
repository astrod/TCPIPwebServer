#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> //file descriptor; file control
#include <unistd.h>
#define BUF_SIZE 100
void error_handling(char* message);

int main(void)
{
	int fd;
	int fdNew;
	char saveBuf[BUF_SIZE] = "";

	// fd=open("data.txt", O_CREAT|O_WRONLY|O_TRUNC); // success : return file Descriptor; fail : return -1;
	// if(fd == -1) // fail;
	// 	error_handling("open() error!");

	// if(write(fd, buf, sizeof(buf)) == -1) //write func parameter: file descriptor, buffer address, file byte;
	// 	error_handling("wirte() error!");

	// close(fd);
	fd = open("data.txt", O_RDONLY);
	if(fd == -1) // fail;
		error_handling("open two() error!");

	if(read(fd, saveBuf, sizeof(saveBuf)) == -1)
		error_handling("read saveBuf Error");

	fdNew = open("dataNew.txt", O_WRONLY|O_TRUNC|O_CREAT);

	if(write(fdNew, saveBuf, sizeof(saveBuf)) == -1)
		error_handling("write fdNew Error");

	close(fd);
	close(fdNew);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
