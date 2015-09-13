#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

char *argv[5] = {"ls", "-l", NULL, NULL, NULL};
char *env[1];
int main(void)
{
	//argv[0] = "aaaaaaaa";
	//env[0] = "./";
	if(fork() !=0 )
	{
		int cp_state;
		printf("I am parent process\n");
		wait(&cp_state);
	}
	else
	{
		execve("/bin/ls", argv, env);
		printf("I am Child precess\n");
	}
	return 0;
}