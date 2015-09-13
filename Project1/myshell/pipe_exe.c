#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pipe_exe.h"
#include <string.h>

//char **cmdvv[MAX_COM_NUM];

/*char *cmd1[4] = {"/bin/ls", "-l", NULL, NULL};
char *cmd2[4] = {"/usr/bin/grep", "main", NULL, NULL};
char *cmd3[4] = {"/usr/bin/grep", "main.c", NULL, NULL};*/
static char *env[1];

//int fdv[2][2];
//int state[3];
//int idt[3];

//int fd;

/**********************************************************************************
 * function name: first_cmd
 * Parametes:     
 *				  1. (*fdv)[FD_IEMS_NUM]  For storage of fd of pipes
 *				  2. index For distinguishing the pipe 
 * intro:         This function will prepare the pipe (written port) for first cmd               
 **********************************************************************************/

static void first_cmd(int (*fdv)[FD_IEMS_NUM], int index)
{
	close(fdv[index][READ_PORT]);
	close(STD_OUTPUT);
	dup(fdv[index][WRITE_PORT]);
	close(fdv[index][WRITE_PORT]);
}

/**********************************************************************************
 * function name: mid_cmd
 * Parametes:     
 *				  1. (*fdv)[FD_IEMS_NUM]  For storage of fd of pipes
 *				  2. index For distinguishing the pipe
 *                3. index For distinguishing the pipe
 * intro:         This function will prepare the pipe (written port) for midium cmd               
 **********************************************************************************/

static void mid_cmd(int (*fdv)[FD_IEMS_NUM], int index1, int index2)
{
	close(fdv[index1][WRITE_PORT]);
	close(STD_INPUT);
	dup(fdv[index1][READ_PORT]);
	close(fdv[index1][READ_PORT]);

	close(fdv[index2][READ_PORT]);
	close(STD_OUTPUT);
	dup(fdv[index2][WRITE_PORT]);
	close(fdv[index2][WRITE_PORT]);
}

/**********************************************************************************
 * function name: last_cmd
 * Parametes:     
 *				  1. (*fdv)[FD_IEMS_NUM]  For storage of fd of pipes
 *				  2. index For distinguishing the pipe 
 * intro:         This function will prepare the pipe (written port) for last cmd               
 **********************************************************************************/

static void last_cmd(int (*fdv)[FD_IEMS_NUM], int index)
{
	close(fdv[index][WRITE_PORT]);
	close(STD_INPUT);
	dup(fdv[index][READ_PORT]);
	close(fdv[index][READ_PORT]);
}

/**********************************************************************************
 * function name: run_pipe1
 * Parametes:     
 *				  1. (*fdv)[FD_IEMS_NUM]  For storage of fd of pipes
 *				  2. cmdvv                Address of cmd name vector
 *                3. env                  no useful
 * intro:         This function will excute two commands by one pipe               
 **********************************************************************************/

void run_pipe1(int (*fdv)[FD_IEMS_NUM], char *cmd1, char *cmd2, char **tep, char **tep2)
{
	int state;

	if(fork())
	{
		wait(&state);
	}
	else
	{
		pipe(fdv[0]);

		if(fork())
		{
			
			wait(&state);		
			last_cmd(fdv, 0);
			
			execve(cmd2, tep2, env);	
		}
		else
		{
				first_cmd(fdv, 0);
				//printf("cmdvv[0] = %s\n", cmdvv[0][0]);
				execve(cmd1, tep, env);
		}
	}

}

/**********************************************************************************
 * function name: run_pipe2
 * Parametes:     
 *				  1. (*fdv)[FD_IEMS_NUM]  For storage of fd of pipes
 *				  2. cmdvv                Address of cmd name vector
 *                3. env                  no useful
 * intro:         This function will excute three commands by two pipe               
 **********************************************************************************/

void run_pipe2(int (*fdv)[FD_IEMS_NUM], char *cmd1, char *cmd2, char *cmd3, char **tep1, char **tep2, char** tep3)
{
	int state;

	if(fork())
	{
		wait(&state);
	}
	else
	{
		pipe(fdv[0]);

		if(fork())
		{
			
			wait(&state);		
			last_cmd(fdv, 0);
			execve(cmd3, tep3, env);	
		}
		else
		{
			pipe(fdv[1]);
			if(fork())
			{
				wait(&state);	
				mid_cmd(fdv, 1, 0);
				execve(cmd2, tep2, env);
			}
			else
			{
				first_cmd(fdv, 1);
				execve(cmd1, tep1, env);
			}
		}
	}

}

void single_cmd(char *cmd_name, char** para, char* path)
{
	int state;
	if(fork())
	{
		wait(&state);
		printf("I am P\n");
	}
	else
	{
		//env[0] = path;
		printf("I am C\n");
		
		
		execve(cmd_name, para, env);
	}
}

/*int main(void)
{
	cmdvv[0] = cmd1;
	cmdvv[1] = cmd2;
	cmdvv[2] = cmd3;

	int state;
	int state2;
	int state3;
	int pid_c;
	//int pid_c2;
	int n = 0;
	//int flag = LEFT;
	

	
	run_pipe2(fdv, cmdvv, env);
	
	return 0;
}*/