#include "init.h"
#include <stdio.h>
#include "main.h"
#include "exit_sig.h"
#include "run.h"

//#include "interpretor_services.h"


int fdv[2][2];
//int state[3];
//int idt[3];
char variable[100] = "x";
char value[100];

int main(void)
{
	init();
	run();
	
	/*write_file(variable,value);
	read_file (variable,value);
	printf("%s\n", value);*/

	return 0;
}