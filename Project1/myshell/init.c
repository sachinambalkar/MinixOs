
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


#define CONFIG_DAT      "./Config.dat"        //the configrated file
#define HOME_DIR_SIZE       16                //the name size of defalut dir: /home
#define BIN_DIR_SIZE        16                //the name size of /bin
#define USR_BIN_DIR_SIZE    16                //the name size of usr/bin
#define MAX_CONF_SIZE       128               //the size of whole config file

static char* point_file = CONFIG_DAT;
static char conf_buf[MAX_CONF_SIZE];
char def_dir[HOME_DIR_SIZE]; //default dir (/home)
char dir_bin[BIN_DIR_SIZE];  ///bin
char dir_usr_bin[USR_BIN_DIR_SIZE]; ///usr/bin
char *cur_dir; //current dir

#define NUM_DIR 3    //3 dir in total


/***************************************************************************
 * function name: init
 * Parametes:     none
 * intro:         This function will 
 *                1. Read the config file to get 3 accessable dir
 *                2. Process the string of dir to divide it into 3 sub-dirs
 *                3. Display the current dir (defualt dir is /home)
 ***************************************************************************/
void init()
{
	int conf_file_fd = open(CONFIG_DAT, O_RDONLY);
	if(conf_file_fd)
	{
		read(conf_file_fd, conf_buf, MAX_CONF_SIZE);
		//printf("fd = %d\n", conf_file_fd);
	}
	else
		printf("Config.dat is opened unsucessfully\n");

//Divide the string of dir into three parts

	int j = 0;
	int n = 0;

	while('\n' != conf_buf[j])
	{
		def_dir[n] = conf_buf[j];
		j++;
		n++;	
	}
	def_dir[n] = '\0';

	//printf("def_dir = %s\n", def_dir);

	n = 0;  //clear n
	j++;

	while('\n' != conf_buf[j])
	{
		dir_bin[n] = conf_buf[j];
		j++;
		n++;	
	}
	dir_bin[n] = '\0';

	//printf("dir_bin = %s\n", dir_bin);

	n = 0;
	j++;

	while('\n' != conf_buf[j])
	{
		dir_usr_bin[n] = conf_buf[j];
		j++;
		n++;	
	}
	dir_usr_bin[n] = '\0';

	//printf("dir_usr_bin = %s\n", dir_usr_bin);

	cur_dir = def_dir;

	
}

