#ifndef _PIPE_EXE_H
#define _PIPE_EXE_H

#define FD_IEMS_NUM 2
#define MAX_COM_NUM 3

#define READ_PORT    0
#define WRITE_PORT   1
#define STD_INPUT    0
#define STD_OUTPUT   1

/***************************************************************************
 * Export functuons
***************************************************************************/
extern void run_pipe1(int (*fdv)[FD_IEMS_NUM], char *cmd1, char *cmd2, char **tep, char **tep2);
extern void run_pipe2(int (*fdv)[FD_IEMS_NUM], char *cmd1, char *cmd2, char *cmd3, char **tep1, char **tep2, char** tep3);

extern void single_cmd(char *cmd_name, char **para, char* path);



#endif