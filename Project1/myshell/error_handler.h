#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

//#define RC int
#define command_not_found 3
#define invalid_command 4
#define parameter_exceeds 5
#define parameter_missing 6

#define FILE_OPEN_FAIL 7
#define INVALID_DIR_ACCESS 8
#define CMDS_NUM_OVERFLOW 9
#define UNKNOW_COMD 10

extern void error_handler(int);

#endif
