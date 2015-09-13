#ifndef _INTERPRETOR_SERVICES_H
#define _INTERPRETOR_SERVICES_H

#define COMMAND_SIZE 100
#define VARIABLE_FILENAME "value_record"

typedef struct pointer
{
	char bracket,command[COMMAND_SIZE];
	struct pointer *next;
}ptr;

extern void get_command(ptr *head,char*s[],char*path);
extern int total_node(ptr *head);
extern void get_command_link(char input[COMMAND_SIZE],ptr *head);
extern void cmd_allocate_space(char **cmdv);
extern void free_node_space(char **cmdv);

extern void write_file (char *var,char *val);
extern void read_file (char *var,char *val);



#endif
