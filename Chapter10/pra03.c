#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

int main(void) {
	pid_t pid;
	int fd1;
	fd1=socket(PF_INET, SOCK_STREAM, 0);
	pid=fork();

	if(pid == 0) printf("childsock : %d \n", fd1);
	else printf("parentsock : %d \n", fd1);
	return 0;
}
