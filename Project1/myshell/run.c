
#include <sys/wait.h>
#include <unistd.h>
#include "exit_sig.h"
#include "init.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "interpretor_services.h"
#include "error_handler.h"
#include "pipe_exe.h"

#define KEEP_RUNNING    1
#define CMD_SIZE        128


#define FIND_SUCESS          1
#define FIND_FAIL            0


#define PATH_SIZE            100

#define NUM_OF_ARGS          4
#define NUM_OF_CMDS          3

#define FIRST_CMD            0
#define SECOND_CMD           1
#define THIRD_CMD            2

#define CMD_NAME_INDEX       0
#define FIRST_ARG            1
#define SECOND_ARG           2
#define THIRD_ARG            3



static char cmd[CMD_SIZE];
static char *cmdv[NUM_OF_ARGS];
static char *cmds[NUM_OF_CMDS][NUM_OF_ARGS];

//=================================== need to change ============================//
static ptr *head;
static char path[PATH_SIZE];
static char *pathv[NUM_OF_CMDS];
//char *c;
static int fdv[2][2];
static char **cmdvv[MAX_COM_NUM];
char *temp_args[NUM_OF_ARGS] = {NULL, NULL, NULL, NULL};
char *temp_args2[NUM_OF_ARGS] = {NULL, NULL, NULL, NULL};
char *temp_args3[NUM_OF_ARGS] = {NULL, NULL, NULL, NULL};
/***************************************************************************
 * function name: dir_search
 * Parametes:     cmd_name, dir_name
 * intro:         This function will find the cmd in specified dir				  
 * return:        if the cmd is found, return sucess, or fail
 ***************************************************************************/
 static int dir_search(char *cmd_name, char *dir_name)
 {
 	int result = FIND_FAIL; //init the result
 	DIR *d = opendir(dir_name); //open a dir
	struct dirent *file = NULL;
	
	while((file = readdir(d)) != NULL)
	{
		if(strncmp(file->d_name, ".", 1) == 0) //skip the current dir, parent dir and hidden file to void infinite loop
			continue;
		//strcpy(fn, file->d_name);
		if(!strcmp(cmd_name, file->d_name)) //find the cmd
		{
			
			result = FIND_SUCESS;
		}
	}
	return result;
 }

/***************************************************************************
 * function name: cmd_search
 * Parametes:     cmd_name
 * intro:         This function will find the cmd that user inputs in 3 paths
 *				  1. /bin
 *                2. /usr/bin
 *                3. /home
 * return:        if the cmd is found, return the path
 ***************************************************************************/

static char* cmd_search(char *cmd_name)
{
	if(dir_search(cmd_name, def_dir))
		return def_dir;
	if(dir_search(cmd_name, dir_bin))
		return dir_bin;
	if(dir_search(cmd_name, dir_usr_bin))
		return dir_usr_bin;

	//exception_service(CMD_NOT_FOUND); //find failed
	return NULL;
}



/***********************************************************************************
 * function name: run
 * Parametes:     none
 * intro:         This function will keep runing except reciving the CRTL+C or exit
 
 * return:        none
 **********************************************************************************/

void run()
{
	//interpretor_init(cmdv);

	//printf("SIZE is %d\n", sizeof(cmds));
	memset(cmds, 0, sizeof(cmds));
	/*for(int i = 0; i < NUM_OF_CMDS; i++)
	{
		for(int j = 0; j < NUM_OF_ARGS; j++)
		{
			cmds[i][j] = NULL;
			//printf("j = %d\n", j);
		}

	}*/

	while(KEEP_RUNNING)
	{
		int i=0;
		char *ret_dir = NULL;
		printf("%s@", cur_dir);
		signal(SIGINT, signal_handler);// exit the shell if CTLR+C

		memset(pathv, 0, sizeof(pathv));
		
        head = (ptr *)malloc(sizeof(ptr));
        
		scanf(" %[^\t\n]",cmd);

		get_command_link(cmd, head);

		int cmd_num = total_node(head); //scan the link table to get num of cmds
		if(cmd_num > 3)
			error_handler(CMDS_NUM_OVERFLOW);

		//fill out the key struct sent to pipe_exe
		ptr* cur_p = head;
		int cmd_index = 0;
        while(cur_p)
		{
			cmd_allocate_space(cmdv);

			get_command(cur_p, cmdv, path);
			//printf("path 1 = %s\n", path);
			//printf("cmd1 name is %s\n", cmdv[0]);
			
			for(int j = 0; j < NUM_OF_ARGS; j++)
			{
				cmds[i][j] = cmdv[j];
				pathv[i] = path;
				//printf("cmds[%d][%d] = %s, path = %s\n", i, j, cmds[i][j], pathv[i]);
			}
			i++;
			//free_node_space(cmdv);
			cur_p = cur_p->next;
		
		}

		//fill_cmd_struct(cmds[NUM_OF_CMDS][NUM_OF_ARGS], pathv[NUM_OF_CMDS]);
		

		if(!(strcmp(cmds[FIRST_CMD][CMD_NAME_INDEX], "cd"))) //the first cmd is change-dir cmd
		{
			if(cmds[FIRST_CMD][FIRST_ARG][0] == 0)// cd + NULL
			{
				//chdir(def_dir); //back to /home
				strcpy(cur_dir, def_dir);
				//printf("first arg = %s\n", cmds[FIRST_CMD][FIRST_ARG]);
			}
			else if(strcmp(cmds[FIRST_CMD][FIRST_ARG], def_dir) 
				&& strcmp(cmds[FIRST_CMD][FIRST_ARG], dir_bin)
				&& strcmp(cmds[FIRST_CMD][FIRST_ARG], dir_usr_bin))
			{
				//printf("Invalid Access!\n");
				error_handler(INVALID_DIR_ACCESS);
			}
			else
			{
				//chdir(cmds[FIRST_CMD][FIRST_ARG]);
				strcpy(cur_dir, cmds[FIRST_CMD][FIRST_ARG]);
			}
			continue;
		} 


		//validate the cmd name
		char s[NUM_OF_CMDS][100];
		char temp_path[NUM_OF_CMDS][50];

		for(i = 0; i < cmd_num; i++) 
		{
			//printf("cmd name is %s\n", cmds[i][CMD_NAME_INDEX]);
			if(!cmds[i][CMD_NAME_INDEX])
				break;
			strcpy(s[i], cmds[i][CMD_NAME_INDEX]);


			ret_dir = cmd_search(s[i]);//find the cmd in path

			if(ret_dir) //cmd is found
			{
				strcpy(pathv[i], ret_dir);
				//printf("The find result is: %s, path[%d] is %s\n", ret_dir, i, pathv[i]);
				strcpy(temp_path[i], pathv[i]);
				int len = strlen(temp_path[i]);
				temp_path[i][len] = '/';
				temp_path[i][len + 1] = '\0';

				//printf("temp_path = %s\n", temp_path[i]);
			}
			else //cmd not found
			{
				//printf("%s is not found! Please check the CMD name!\n", s[i]);//
				error_handler(UNKNOW_COMD);
			}
			ret_dir = NULL;
		}

		
		

		for(i = 0; i < NUM_OF_ARGS; i++)
		{
			temp_args[i] = cmds[FIRST_CMD][i];
				//printf("arg = %s, leng = %d\n", temp_args[i], strlen(temp_args[i]));
		}

		for(int k=0;k<4;k++)
		{
			if(*(temp_args[k]+0)=='\0')
				temp_args[k]=NULL;
		
		}

		if(cmd_num == 2 || cmd_num == 3)
		{
			for(i = 0; i < NUM_OF_ARGS; i++)
			{
				temp_args2[i] = cmds[SECOND_CMD][i];
					//printf("arg = %s, leng = %d\n", temp_args2[i], strlen(temp_args2[i]));
			}

			for(int k=0;k<4;k++)
			{
				if(*(temp_args2[k]+0)=='\0')
					temp_args2[k]=NULL;
		
			}
		}
		
		if(cmd_num == 3)
		{
			for(i = 0; i < NUM_OF_ARGS; i++)
			{
				temp_args3[i] = cmds[THIRD_CMD][i];
					//printf("arg = %s, leng = %d\n", temp_args3[i], strlen(temp_args3[i]));
			}

			for(int k=0;k<4;k++)
			{
				if(*(temp_args3[k]+0)=='\0')
					temp_args3[k]=NULL;
		
			}
		}
		
		

		for(int i = 0; i < cmd_num; i++)
		{
			strcpy(s[i], cmds[i][CMD_NAME_INDEX]);
			char t[100];
			strcat(t, temp_path[i]);
			strcat(t, s[i]);
			strcpy(s[i], t);
			memset(t, 0, sizeof(t));
			//printf("cmd name = %s\n", s[i]);
		}



		if(cmd_num == 1)
			single_cmd(s[FIRST_CMD], temp_args, temp_path[0]);
		else if(cmd_num == 2)
		{
			/*printf("cmdvv[0] = %s\n", cmdvv[0][0]);
			printf("cmdvv[1] = %s\n", cmdvv[1][0]);
			printf("s[0] = %s\n", s[0]);
			printf("s[1] = %s\n", s[1]);*/
			run_pipe1(fdv, s[0], s[1], temp_args, temp_args2);
			;
			
		}
		else if(cmd_num == 3)
		{
			run_pipe2(fdv, s[0], s[1], s[2], temp_args, temp_args2, temp_args3);
		}
		else
		{
			error_handler(CMDS_NUM_OVERFLOW);
		}
	}
}