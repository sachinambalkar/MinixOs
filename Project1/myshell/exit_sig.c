#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define CRTL_C 2

void signal_handler(int signo)
{
	if(CRTL_C == signo)
	{
		printf("\nAre you sure?[y/n]:");
		char opt;
		scanf("%c", &opt);
		if('y' == opt || 'Y' == opt)
			exit(0);
	}
}