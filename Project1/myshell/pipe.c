#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LEFT 0
#define RIGHT 1

char **cmdvv[3];
char *cmd1[4] = {"/bin/ls", "-l", NULL, NULL};
char *cmd2[4] = {"/usr/bin/grep", "main", NULL, NULL};
char *cmd3[4] = {"/usr/bin/grep", "main.c", NULL, NULL};
char *env[1];

char buf[512];

int main(void)
{
	cmdvv[0] = cmd1;
	cmdvv[1] = cmd2;
	cmdvv[2] = cmd3;
	int fd[2];
	//char buf[1024];
	pipe(fd);
	int state;
	//int state2;
	int pid_c;
	//int pid_c2;
	int n = 0;
	int flag = LEFT;
	while(n < 2)
	{
		pid_c = fork();
		if(pid_c > 0)
		{
			waitpid(pid_c, &state, 0);
			printf("I am P process.\n");
			/printf("C%d process ends.\n", n);
			sleep(1);
			//char buf[512];
			//read(fd[0], buf, 512);
			//printf("buf is %s\n", buf);
			//pid_c2 = fork();
			//if(pid_c2 > 0)
		
			/*close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			execl("/usr/bin/grep", "grep", "test", NULL);*/
			/*close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			read(0, buf, 512);
			printf("buf is %s\n", buf);*/
			printf("Round %d is over\n", n);
			n++;
		}
		else
		{
			if(n == 0)
			{
				printf("I am C%d.\n", n);
				n++;
				close(fd[0]);
				close(1);	
				dup(fd[1]);		
				close(fd[1]);
				execve(cmdvv[n][0], cmdvv[n], env);	
			}	
			else
			{
				//printf("I am C%d\n", n);
				n++;
				close(fd[1]);
				close(0);
				dup(fd[0]);
				close(fd[0]);
				//char s[512] = "main.c";
				//read(0, buf, 512);
				//printf("xxxxxxxxxxxxxxxxxxxxxx\n");
				//printf("buf = %s\n", buf);
				execve(cmdvv[0][0], cmdvv[n], env);
			}
			
		}
	}

	return 0;
}