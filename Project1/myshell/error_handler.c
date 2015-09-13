#include "error_handler.h"
#include "run.h"
#include<stdio.h>



/*void error_handler(int RC_value)
{
	if(RC_value==command_not_found)
		printf("\nCommand Not found\n");
	else if(RC_value==invalid_command)
		printf("\nInvalid input\n");
	else if(RC_value==parameter_exceeds)
		printf("\nParameter limit exceeds. Limit is 3.\n");
	else if(RC_value==parameter_missing)
		printf("\nCommand parameter missing\n");
	else
		printf("\nError");


	}*/

	static void (*fp)(void) = run;

	void error_handler(int RC_value)
	{
		switch(RC_value)
		{
			case command_not_found: 
			{
				printf("\nCommand Not found\n");
				break;
			}
			case invalid_command: 
			{
				printf("\nInvalid input\n");
				break;
			}
			case parameter_exceeds: 
			{
				printf("\nparameter exceeds\n");
				break;
			}
			case parameter_missing: 
			{
				printf("\nparameter missing\n");
				break;
			}
			case FILE_OPEN_FAIL: 
			{
				printf("\nFILE OPEN FAIL\n");
				break;
			}
			case INVALID_DIR_ACCESS: 
			{
				printf("\nINVALID DIR ACCESS\n");
				break;
			}
			case CMDS_NUM_OVERFLOW: 
			{
				printf("\nIt is not allow to input more than 3 commands.\n");
				break;
			}
			case UNKNOW_COMD: 
			{
				printf("\nThe CMD you input cannot be found! Please check it and input again!\n");
				break;
			}
		}
		(*fp)();
	}
