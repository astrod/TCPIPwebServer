/*
 * =====================================================================================
 *
 *       Filename:  dup.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/26/2014 06:12:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int cfd1, cfd2;
	char str1[] = "HI ~ \n";
	char str2[] ="It's a nice day \n";

	cfd1 =dup(1);
	cfd2 = dup2(cfd1, 7);

	printf("fd1=%d, fd2=%d \n", cfd1, cfd2);
	write(cfd1, str1, sizeof(str1));
	write(cfd2, str2, sizeof(str2));

	close(str1);
	close(str2);
	write(1, str1, sizeof(str1));
	close(1);
	write(1, str2, sizeof(str2));

	return 0;
}
