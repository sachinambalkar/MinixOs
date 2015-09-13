#ifndef _INIT_H
#define _INIT_H


#define CONFIG_DAT      "./Config.dat"        //the configrated file
#define HOME_DIR_SIZE       16                //the name size of defalut dir: /home
#define BIN_DIR_SIZE        16                //the name size of /bin
#define USR_BIN_DIR_SIZE    16                //the name size of usr/bin
#define MAX_CONF_SIZE       128               //the size of whole config file

/************************************
 * Export vars
*************************************/
extern char def_dir[HOME_DIR_SIZE]; //default dir (/home)
extern char dir_bin[BIN_DIR_SIZE];  ///bin
extern char dir_usr_bin[USR_BIN_DIR_SIZE]; ///usr/bin
extern char *cur_dir; //current dir

/*************************************
 * Export functuons
**************************************/
extern void init(void);

#endif