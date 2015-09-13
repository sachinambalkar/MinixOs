
#include "pm.h"
#include <minix/callnr.h>
#include <signal.h>
#include <sys/svrctl.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <minix/com.h>
#include <minix/config.h>
#include <minix/sysinfo.h>
#include <minix/type.h>
#include <minix/vm.h>
#include <string.h>
#include <machine/archtypes.h>
#include <lib.h>
#include <assert.h>
#include "mproc.h"
#include "param.h"
#include"newSystemcall.h"
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include <lib.h>
#include "error_code.h"
#include<malloc.h>



typedef struct subscriber_data
{
	pid_t sid,p_id[MAXIMUM],tid[MAXIMUM];
	int count;
}sdata;

typedef struct message_topic
{
	int mid,total_msg,add_index,read_index;
	char message[5][MAXIMUM];
	char name[100];
	pid_t check[5][MAXIMUM];
}mdata;

typedef struct topic_details
{
	int tid;
	char name[100];
}tdata;

static tdata topic_lookup[MAXIMUM];
static int tcount=0;

typedef struct publisher_data
{
	pid_t p_id;
	int tid[100];
	int tcount;
}pdata;

static pdata publisher_lookup[MAXIMUM];
static int pcount=0;


static sdata subscriber_lookup[MAXIMUM];
static int scount=0;

static mdata message_lookup[MAXIMUM];
static int mcount=0;




void init()
{

	int i,j,k;
	for(i=0;i<MAXIMUM;i++)
	{

		publisher_lookup[i].p_id=-1;
		publisher_lookup[i].tcount=0;
		pcount=0;

		topic_lookup[i].tid=-1;
		topic_lookup[i].name[0]='\0';
		tcount=0;

		subscriber_lookup[i].count=0;
		subscriber_lookup[i].sid=-1;
		for(j=0;j<MAXIMUM;j++)
		{
			subscriber_lookup[i].tid[j]=-1;
			subscriber_lookup[i].p_id[j]=-1;
			publisher_lookup[i].tid[j]=-1;
		}


		message_lookup[i].mid=0;
		message_lookup[i].total_msg=0;
		message_lookup[i].add_index=0;

		for(j=0;j<5;j++)
		{
			message_lookup[i].message[j][0]='\0';
			for(k=0;k<MAXIMUM;k++)
				message_lookup[i].check[j][k]=-1;
		}
	}
}


int Topic_create(char name_p[])
{
	int i;
	for(i=0;i<tcount;i++)
	{
		if(strcmp(topic_lookup[i].name,name_p)==0)
		{
		//	printf("Topic is already exist..!!!");
			return -5;
		}
	}
	strcpy(topic_lookup[i].name,name_p);
	topic_lookup[i].tid=tcount;
	tcount++;
	return 99;
}

int get_TOPIC_ID(char name_p[])
{
		int i;
		for(i=0;i<tcount;i++)
		{
		//	printf("search %s %s",topic_lookup[i].name,name_p);
			if(strcmp(topic_lookup[i].name,name_p)==0)
				return topic_lookup[i].tid;
		}
		return -6;
}




int Topic_publish(char name_p[],pid_t current_pid)
{
	int tid,i;
	//=getpid();
	tid=get_TOPIC_ID(name_p);
	//printf("topic_id = %d",tid);

	if(tid==-6)
	{
		//printf("Returned -6");
		return -6;
	}
	for(i=0;i<pcount;i++)
	{
			if(publisher_lookup[i].p_id==current_pid)
				break;
	}
	if(i<pcount)
		publisher_lookup[i].tid[publisher_lookup[i].tcount]=tid;
	else
	{
		publisher_lookup[i].p_id=current_pid;
		publisher_lookup[i].tid[publisher_lookup[i].tcount]=tid;
		pcount++;
	}
	publisher_lookup[i].tcount++;

	return 99;
}


int Topic_subsciber(char name_p[],pid_t current_process)
{

	int found=0,k,i,alreadyexist=0,insert_tid;
	//=getpid();


		insert_tid=get_TOPIC_ID(name_p);
		//printf("\nTS RV=%d",insert_tid);
		if(insert_tid==-6)
		{

		//	printf("\ninside insert_tid..!!");
			return -6;
		}


	for(i=0;i<scount;i++)
	{
		if(subscriber_lookup[i].sid==current_process)
		{
			found=1;
			break;
		}
	}
	if(found==1)
	{
		for(k=0;k<subscriber_lookup[i].count;k++)
		{
				if(subscriber_lookup[i].tid[k]==insert_tid)
				{
//					printf("\nTopic is already Subscribed..!!");
					alreadyexist=1;
					break;
				}
		}
		if(alreadyexist!=1)
		{
			subscriber_lookup[i].tid[k]=insert_tid;
			subscriber_lookup[i].count++;
		}
	}
	else
	{
		subscriber_lookup[scount].sid=current_process;
		subscriber_lookup[scount].tid[0]=insert_tid;
		subscriber_lookup[scount].count++;
		scount++;
	}

	return 99;
}




int interseted[10]={0,0,0,0,0,0,0,0,0,0};
int turn=0;
int thisProcessNumber=-1;

void EnterCriticalSection(int thisProcess)
{
	int otherProcess;
	if(thisProcess>2)
		otherProcess=thisProcess-1;
	else
		otherProcess=1-thisProcess;
	interseted[thisProcess]=1;
	turn=thisProcess;
	while((turn==thisProcess)&&(interseted[otherProcess]));
	printf("\nEntered Critical Region...!!!\n");
}

void LeaveCriticalSection(int thisProcess)
{
	interseted[thisProcess]=0;
	printf("\nReleased Critical Region...!!!\n");
}


int getProcessNumb()
{
	thisProcessNumber++;
	return thisProcessNumber;
}



int publish_message(char message[],pid_t current_pid)
{
	int thisProcess=getProcessNumb();
	EnterCriticalSection(thisProcess);

	int mid,i,found,index,j;
			int h,w,valid_publisher=0;

			//=getpid();
				for(w=0;w<pcount;w++)
				{
					if(publisher_lookup[w].p_id==current_pid)
					{
						valid_publisher=1;
						break;
					}
				}
			if(valid_publisher==0)
			{
				printf("\nYou are not registered as publisher.Can't Publish message.!!!!");
				LeaveCriticalSection(thisProcess);
				return -7;
			}
			else
			{
				for(h=0;h<publisher_lookup[w].tcount;h++)
				{

			/////////////////
						mid=publisher_lookup[w].tid[h];
						{
								found=0;
								for(i=0;i<mcount;i++)
								{
									if(message_lookup[i].mid==mid)
									{
										found=1;
										break;
									}
								}
								if(found==1)
								{
									index=message_lookup[i].add_index;
									index++;
									if(!(index<5))
									{
										index=addNewMessage(mid);
										if(index==-123)
										{
											printf("\nBuffer is full. Can't publish msg.\n");
											LeaveCriticalSection(thisProcess);
											return -1;
										}
									}
									for(j=0;j<100;j++)
												message_lookup[i].check[index][j]=-1;
									for(j=0;message[j]!='\0';j++)
												message_lookup[i].message[index][j]=message[j];
									message_lookup[i].message[index][j]='\0';
									if(message_lookup[i].total_msg<5)
										message_lookup[i].total_msg++;
									message_lookup[i].add_index++;
								}
								else
								{
//									printf("\ninside %d",h);
										message_lookup[i].total_msg=1;
										message_lookup[i].add_index=0;
										message_lookup[i].read_index=0;
										message_lookup[i].mid=mid;
										for(j=0;j<100;j++)
											message_lookup[i].check[0][j]=-1;

									//	printf("\n==>%s ",message);

										for(j=0;message[j]!='\0';j++)
											message_lookup[i].message[0][j]=message[j];
										message_lookup[i].message[0][j]='\0';
					//					printf("\nm==>%s ",message_lookup[i].message[0]);

										mcount++;
								}
						}
			///////////
					}
			}
			printf("\nMessage published successfully...!!!!\n");
			LeaveCriticalSection(thisProcess);
			return 99;
}

int addNewMessage(int topic_id)
{int i,j,m,found=0,k,count_sub=0;
	for(i=0;i<scount;i++)
	{
			for(j=0;subscriber_lookup[i].tid[j]!=-1;j++)
			{
				if(subscriber_lookup[i].tid[j]==topic_id)
				{
					count_sub++;
					found=1;
					break;
				}
			}
	}

	for(i=0;i<mcount;i++)
	{	if(message_lookup[i].mid==topic_id)
			break;
	}
	for(k=0;k<5;k++)
	{
		for(m=0;message_lookup[i].check[k][m]!=-1;m++);
		if(m>=count_sub)
			return k;
	}
	return -123;
}


int get_message(char *cd,pid_t current_pid)
{
	int i,j,msg_id,k=99;
	char msg[100];
	for(i=0;i<scount;i++)
	{
		if(current_pid==subscriber_lookup[i].sid)
			break;
	}
//	printf("\n i=%d val = %d",i,subscriber_lookup[i].count);
	if(subscriber_lookup[i].count>0)
	{
			for(j=0;j<subscriber_lookup[i].count;j++)
			{
					msg_id=subscriber_lookup[i].tid[j];	//get_messageid(subscriber_lookup[i].tid[j]);
					{
						k=process_msgid(msg_id,msg,current_pid);
							if(k==SUCCESS)
							{
								for(i=0;msg[i]!='\0';i++)
								{
									*(cd+i)=msg[i];
								}
								*(cd+i)='\0';
								return k;
							}
					}
			}
			if(msg_id==ERROR_NOTFOUND)
				return -2;
	}
	else
	{
			return -3;
	}


	return -1;

}
int process_msgid(int message_id,char *str,pid_t para_pid)
{
	int i,j,k,m,data_already_read=0;
	for(i=0;i<mcount;i++)
	{
		if(message_lookup[i].mid==message_id)
			break;
	}

	if(message_lookup[i].total_msg>0)
	{
			for(j=0;j<5;j++)
			{
				data_already_read=0;
				if(message_lookup[i].message[j][0]!='\0')
				{
					for(k=0;k<10;k++)
					{
						if(para_pid==message_lookup[i].check[j][k])
						{
								data_already_read=1;
								break;
						}
					}

					if(data_already_read==0)
					{
							for(m=0;message_lookup[i].message[j][m]!='\0';m++)
							{
								*(str+m)=message_lookup[i].message[j][m];
							}
							*(str+m)='\0';
							for(m=0;message_lookup[i].check[j][m]!=-1;m++);
							message_lookup[i].check[j][m]=para_pid;
							return 99;
					}
				}
			}
	}
	else
	{
		return -4;
	}
	return -1;
}


void get_topic_name(int topic_id,char *topic_name)
{
	int i,k;
	*(topic_name+0)='\0';
	for(i=0;i<tcount;i++)
	{
		if(topic_lookup[i].tid==topic_id)
		{
			for(k=0;topic_lookup[i].name[k]!='\0';k++)
				*(topic_name+k)=topic_lookup[i].name[k];
			*(topic_name+k)='\0';
			break;
		}
	}

}

void Topic_lookup()
{
		int i;
		if(tcount>0)
		{
			printf("\n\n@@@@ Topics Available @@@@@@@@@@@@@@@@@@");
			for(i=0;i<tcount;i++)
				printf("\n%d.	%s",topic_lookup[i].tid,topic_lookup[i].name);
			printf("\n@@@@@@@@@@@ End @@@@@@@@@@@@@@@@@@@@@@@@\n\n");
		}
		else
			printf("\nTopic data is empty\n");
}

void show_subscribe()
{
	int i=0,k=0;
	if(scount>0)
	{
			printf("\n\n@@@@@@@@@ Subscriber Details @@@@@@@@@@@");
			for(i=0;i<scount;i++)
			{
				printf("\n%d. Subscriber id: %d \nTotal subscribed topics are %d\nTopics Details=>\n",i+1,(int)subscriber_lookup[i].sid,subscriber_lookup[i].count);
				for(k=0;k<subscriber_lookup[i].count;k++)
				{
					char topic_name[100];
					get_topic_name(subscriber_lookup[i].tid[k],topic_name);
					printf("%d = %s	  ",k+1,topic_name);
				}
				printf("\n");
			}
			printf("\n@@@@@@@@@@@@@ End @@@@@@@@@@@@@@@@@@@@@@\n");
	}
	else
		printf("\nSubscriber data is empty");
}

void show_publish()
{
	int i=0,j;
	if(pcount>0)
	{
		printf("\n@@@@@@@@ Publisher Details @@@@@@@@@@\n");
		printf("\nNumber\tProcess-ID");
		for(i=0;i<pcount;i++)
		{
			printf("\n%d\t%d\n",i+1,publisher_lookup[i].p_id);
			printf("This process is subscribed to following topics=>\n");
			printf("\tNumber\tTopic_Name");
			for(j=0;j<publisher_lookup[i].tcount;j++)
			{
				char topic_name[100];
				get_topic_name(publisher_lookup[i].tid[j],topic_name);
				printf("\n\t%d\t%s",j+1,topic_name);
			}
		}
		printf("\n@@@@@@@@@@@@@ End @@@@@@@@@@@@@@@@@@@@@@\n");
	}
	else
		printf("\nPublisher Data is empty");
}

void show_message_topic()
{
	int i,j,k;
	for(i=0;i<mcount;i++)
	{
		printf("\n %d. Topic Id = %d \n@@@@@@ Message Details @@@@@@\n",i+1,message_lookup[i].mid);
		for(j=0;j<message_lookup[i].total_msg;j++)
		{
			if(message_lookup[i].check[j][0]!=-1)
			{printf("\n%d.\tMessage:  %s  \n@@@@ Process that read this message as follows=>@@@\n",j+1,message_lookup[i].message[j]);
			printf("Number\tProcess-ID");}
			for(k=0;message_lookup[i].check[j][k]!=-1;k++)
			printf("\n%d.\t%d\n",k+1,(int)message_lookup[i].check[j][k]);
			printf("\n");
		}
	}
}


int do_topic_publisher()
{
	char name_p[100];
	int a,Return_val;
	strcpy(name_p,m_in.m3_ca1);
	a=m_in.m1_i1;
	Return_val=Topic_publish(name_p,a);
	//printf("\nRV=%d",Return_val);
	return Return_val;
}

int do_topic_subscriber()
{
	char name_p[100];
	int a,Return_val;
	strcpy(name_p,m_in.m3_ca1);
	a=m_in.m1_i1;
	Return_val=Topic_subsciber(name_p,a);
	return Return_val;
}

int do_topic_publish_msg()
{
	char name_p[100];
	int a;
	strcpy(name_p,m_in.m3_ca1);
	a=m_in.m1_i1;
	publish_message(name_p,a);
	return 99;
}

int do_createtopic()
{
	char name_p[100];
	strcpy(name_p,m_in.m3_ca1);
//	printf("\nTopic Received=> %s  \n",name_p);

	int i;
	for(i=0;i<tcount;i++)
	{
		if(strcmp(topic_lookup[i].name,name_p)==0)
		{
	//		printf("Topic is already exist..!!!");
			return -5;
		}
	}
	strcpy(topic_lookup[i].name,name_p);
	topic_lookup[i].tid=tcount;
	tcount++;
	return 99;
}


int do_mysyscall()
{
	//int a=m_in.m1_i1;
//	printf("This is my First system call. Value Received=%d \n",a);

	Topic_lookup();
	show_subscribe();
	show_publish();
//	show_message_topic();

	return 99;
}

int do_display_msg_table(void)
{
	show_message_topic();
	return 99;
}

int do_display_sub(void)
{
	show_subscribe();
	return 99;
}
int do_display_pub(void)
{
	show_publish();
	return 99;
}
int do_display_topic(void)
{
	Topic_lookup();
	return 99;
}


int do_init(void)
{
//	printf("\ninside do_init..\n");
	init();
	return 99;
}


int do_receive_msg(void)
{
	char msg[100];
	int a=m_in.m1_i1,RV;
//@@@@@@@@	RV=get_message(&msg,a);
	RV=get_message(msg,a);
	if(RV>0)
	{
		printf("\n %d Message Received is =%s\n",RV,msg);
		return 99;
	}else
		return RV;
}
