#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	int i;
	int fds1[2], fds2[2];
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);
	pid=fork();
	if(pid == 0) {
		for(i=0; i<3; i ++) {
			fputs("insert message1 : ", stdout);
			fgets(buf, sizeof(buf), stdin);
			write(fds1[1], buf, sizeof(buf));
			read(fds2[0], buf, BUF_SIZE);
			printf("received word from process2: %s", buf);
		}
	}
	else {
		for(i=0; i<3; i ++) {
			read(fds1[0], buf, BUF_SIZE);
			printf("received word from process1: %s", buf);
			fputs("insert message2 : ", stdout);
			fgets(buf, sizeof(buf), stdin);
			write(fds2[1], buf, sizeof(buf));
		}
	}


	return 0;
}