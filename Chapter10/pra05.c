#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void endMain(int sig) {
	char saveExit;
	if(sig == SIGINT) puts("Do you want to exit?");
	scanf("%c", &saveExit);
	if(saveExit == 'Y' || saveExit == 'y')
	exit(0);

}

int main(void) {
	signal(SIGINT, endMain);
	while(1) {
		printf("this is a string \n");
		sleep(1);
	}
	return 0;
}