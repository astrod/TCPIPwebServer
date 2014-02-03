#include <stdio.h>
#include <stdlib.h>
void error_handling(char* message);


int main(void)
{
	FILE *fp1 = fopen("data.txt", "r");
	FILE *fp2 = fopen("data_Copy_High.txt", "w");
	char buf[100] = "";
	int input = 0;

	if(fp1 == NULL || fp2 == NULL) 
		error_handling("create file error!");

	while(input != EOF) {
		input = fgetc(fp1);
		fputc(input, fp2);

	}

	fclose(fp1);
	fclose(fp2);



	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}