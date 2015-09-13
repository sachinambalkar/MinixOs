#include "interpretor_services.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include"error_handler.h"

#define NUM_OF_ARGS          4

void cmd_allocate_space(char **cmdv)
{
	for(int i=0;i<NUM_OF_ARGS;i++)
	{
		cmdv[i]=(char*)malloc(4);
		//*(cmdv[i])=(char*)NULL;
	}
}

void free_node_space(char **cmdv)
{
	for(int i=0;i<NUM_OF_ARGS;i++)
		free(cmdv[i]);
}

void clear_node(ptr *head)
{
	while(head)
	{

		head=head->next;
	}
}

void write_file (char *var,char *val)
{
		int fd1,i=0,k=0;
	    char buf[128];
	    char filename[20]=VARIABLE_FILENAME;
	    fd1 = open(filename, O_WRONLY|O_APPEND|O_CREAT);
	    if (fd1 == -1)
	    {
	    	printf("\nFile can't open");
	    	error_handler(FILE_OPEN_FAIL);
	    }
	    else
	    {
	    	buf[0]=' ';
	    	for(i=0;*(var+i)!='\0';i++)
	    			buf[i+1]=*(var+i);
	    	buf[i+1]='=';i++;
	    	for(;*(val+k)!='\0';k++,i++)
	    		buf[i+1]=*(val+k);
	    	buf[i+1]=' ';
	    	buf[i+2]='\0';
	    	write(fd1, buf,i+2);
		    close(fd1);
	    }
}

void read_file (char *var,char *val)
{
		char* fileName=VARIABLE_FILENAME;
		int j=0,operation_done=0,position=0,i=0,count=0,pipe,val1=0,val2=0,k=0,identical_char=0;
	    char variable[100][100];
	    char *ch=(char*)malloc(sizeof(char)),*start=(char*)malloc(sizeof(char));
	    pipe = open(fileName,O_RDONLY);
	    if (pipe < 0)
	    {
	    	close(creat(fileName,O_CREAT));
	    }
	    while (1)
	    {
				if(read(pipe,ch+i,1)>0)
					i++;
				else
					break;
	    }
	    close(pipe);
	    (*(ch+i))='\0';
//	    printf("\nFile content: \n%s",ch);
	    identical_char=0;
	    position=0;
	    operation_done=0;
	    *(val+0)='\0';
	    for(i=0;*(ch+i)!='\0';i++)
	    {
		   if(*(ch+i)==*(var+0))
		   {
			   identical_char=1;
			   k=i;
			   position=0;
			   while(identical_char==1)
			   {
					   if((*(ch+k+1)=='=')&&(*(var+position+1)=='\0')&&identical_char==1)
					   {
						   	   operation_done=1;
	//					   	   printf("\nFound\n");
						   	   k++;k++;
						   	   j=0;
						   	   for(;*(ch+k)!='\0'&&*(ch+k)!=' ';k++,j++)
						   		   *(val+j)=*(ch+k);
						   	*(val+j)='\0';
							   break;
					   }
					   if(*(ch+k)==*(var+position))
						   identical_char=1;
					   else
						   identical_char=0;

					   if(identical_char==0)
						   break;
					   k++;
					   position++;
			   }
		   }
		   else
			   	   identical_char=0;
		   if(operation_done==1)
			   break;
	   }
	   if(operation_done==1)
		   	   printf("\nFound Value");
	   else
		   	   printf("\nValue not found");
	    close(pipe);
}



int total_node(ptr *head)
{
	ptr *temp;
	int i=0;
	temp=head;
	while(temp->next!=NULL)
	{
		temp=temp->next;
		i++;
	}
	return ++i;

}


void get_command(ptr *head,char*s[],char*path)
{
	char *variable=(char*)malloc(sizeof(char));
	char *value=(char*)malloc(sizeof(char));
	int val=-1,j=0,i=0,k=0,count=0,start=0,end=0;
	char temp[4][100];
	char systemPath[]="./";
	for(j=0;head->command[j]!='\0'&&head->command[j]!=' ';j++)
	{
		if(head->command[j]=='/')
			val=j;
	}
	if(val!=-1)
	{
		k=0;
		for(j=0;j<=val;j++,k++)
		{
			*(path+j)=head->command[j];
			*(s[count]+k)=head->command[j];
		}
		*(path+j)='/';
		*(path+j+1)='\0';
		*(s[count]+k)='\0';
		val++;
	}
	else
	{
		k=0;
		val=0;
		for(i=0;systemPath[i]!='\0';i++,k++)
		{
			*(path+i)=systemPath[i];
			*(s[count]+k)=systemPath[i];
		}
		*(path+i)='/';
	//	printf("!!!!thsi: %c\n",*(path+i));
		*(path+i+1)='\0';
		*(s[count]+k)='\0';
	}
k=0;
i=0;

	if(head->command[i]==' ')
	{	while(head->command[i]==' ')
		{
			i++;
			if(head->command[i]=='\0')
				break;
		}
	//	i--;
	}
	for(;i<100;i++,k++)
	{
		if(head->command[i]!='\0'&&head->command[i]!=' ')
			*(s[count]+k)=head->command[i];
		else
		{
			*(s[count]+k)='\0';
			if(count<3)
			{
				count++;
				k=-1;
			}
			if(head->command[i]=='\0')
			  break;
			while(head->command[i]==' ')
			{
				i++;
				if(head->command[i]=='\0')
					break;
			}
			i--;
		}
	}

	//printf("count: %d",count);
	while(count<4)
	{
	//	printf("count: %d",count);
		*(s[count])='\0';
		count++;
	}

	//printf("\nThis is it");


	for(i=1;i<4;i++)
		if(*(s[i]+0)=='$')
		{
			*(variable+0)='\0';
			*(value+0)='\0';
			k=0;
			for(j=1;*(s[i]+j)!='\0';j++,k++)
				*(variable+k)=*(s[i]+j);

			*(variable+k)='\0';
			printf("\nVariable: %s Value: %s",variable,value);
			read_file(variable,value);

			if(*(value+0)!='\0')
			{
				for(j=0;*(value+j)!='\0';j++)
					*(s[i]+j)=*(value+j);
			}
			*(s[i]+j)='\0';
//			printf("\nFound variable at %d\n",i);
		}

}





static void parenthesis(ptr *head,char* c)
{
	ptr *start,*temp,*search,*previous;
	int i,braces=0,start_index=0,end_index=-1,j,val=0,count=0;
	char isbraces='n';
	for(i=0;;i++)
	{
		if(c[i]=='(')
		{
			braces++;
			i++;
			if(c[i]!=')'&&c[i]!='\0'&&c[i]!=',')
				start_index=i;
		//	else
		//		error(0);
			i--;
		}
		else if(c[i]==')')
		 {
			if(!(braces%2))
				if(c[i+1]==','||c[i+1]==')')
				{
					isbraces='y';
					braces--;
				}
		}
		else if(c[i]==','||isbraces=='y'||c[i]=='\0')
		{
		      end_index=i-1;
		      if(start_index!=-1&&end_index!=-1)
		      {
			temp=(ptr*)malloc(sizeof(ptr));
			val=0;
			for(j=start_index;j<=end_index;j++,val++)
			{
				if(c[j]==')')
					break;
				temp->command[val]=c[j];
			}
			temp->command[val]='\0';
			temp->bracket=isbraces;
			temp->next=NULL;
			if(count==0)
			{  	start=temp;
				count++;
			}
			else
			{       search=start;
				if(temp->bracket=='n')
				{
				   while(search->next!=NULL)
				    {	search=search->next;   }
				   search->next=(ptr*)malloc(sizeof(ptr));
				   search->next=temp;
				}
				else
				{
					if(search->bracket=='y')
					{
					     while(search->bracket=='y'&&search->next!=NULL)
					     {
					     previous=search;
					     search=search->next;
					     }
					     if(search->bracket=='n')
					     {
						previous->next=temp;
						temp->next=(ptr*)malloc(sizeof(ptr));
						temp->next=search;
					     }
					     else if(search->next==NULL)
					     {
					       search->next=(ptr*)malloc(sizeof(ptr));
					       search->next=temp;
					     }
					}
					else
					{
					     temp->next=(ptr*)malloc(sizeof(ptr));
					     temp->next=search;
					     start=temp;
					}
				}
			}
		      }
		      if(c[i+1]!='\0'&&c[i+1]!='('&&c[i+1]!=')')
		      start_index=i+1;
		      end_index=-1;
		      isbraces='n';
		}
		if(c[i]=='\0')
			break;
	}

	for(i=0;start->command[i]!='\0';i++)
	{
		head->command[i]=start->command[i];
//		printf(" %c ",head->command[i]);
	}
		head->command[i]='\0';
	head->bracket=start->bracket;
	head->next=start->next;
}
/*
static void sigproc()
{
		char choice;
		//signal(SIGINT, sigproc);
		printf("\nroot/usr: ");
		 printf("Do you want to exit (y/n): ");
		 scanf(" %c",&choice);
		 if(choice=='y')
			 	 exit(0);
		 else
			 printf("\nroot/usr: ");
}*/
void get_command_link(char input[COMMAND_SIZE],ptr *head)
{
	int nodeCount=0,i,flag=0,mode=0,v=0,tools=0,j=0;
	char *s[4],*c,*varPointer,*ch,abc,var[100][100],tvar[100],tval[100],*path;
	ptr *start;

	//	signal(SIGINT, sigproc);
		c=(char*)malloc(sizeof(char));
		varPointer=(char*)malloc(sizeof(char));
		ch=(char*)malloc(sizeof(char));
		start=(ptr*)malloc(sizeof(ptr));

		for(i=0;i<4;i++)
		s[i]=(char*)malloc(sizeof(char));

			printf("\n");
			c=&input[0];
			if(c[0]=='(')
					parenthesis(head,c);
			else
			{
					for(i=49;i<=57;i++)
					if(c[0]==i)
					{
						error_handler(command_not_found);
						flag=1;
						break;
					}
					if(flag==0)
					{
							i=0;mode=0;v=0;
							while(c[i]!='\0')
							{
								 if(c[i]==' ')
									break;
								 if(c[i]=='=')
								 {
										if(c[i+1]!=' '&&c[i+1]!='\0')
										{  	mode=1;
											break;
										}
								  }
								  tvar[i]=c[i];
								  i++;
							}
							if(mode==1)
							{
								tvar[i]='\0';v=0;i++;
								while(c[i]!='\0')
								{
									if(c[i]!=' ')
										tval[v]=c[i];
									else
									{
										error_handler(invalid_command);
//										return 1;
									}
									i++;v++;
								}
								tval[v]=' ';
								tval[++v]='\0';
								printf("\nEqual mode");
								printf("\ntvar= %s tval= %s",tvar,tval);
							}
							else
							{
								
								for(j=0;c[j]!=0;j++)
									head->command[j]=c[j];
								head->command[j]='\0';
								head->next=NULL; 
								

							}
					}
			}

}
