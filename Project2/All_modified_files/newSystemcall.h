#include<sys/types.h>
#include <lib.h>
#include <unistd.h>
#include<stdio.h>
#ifndef NEWSYSTEMCALL_H
#define NEWSYSTEMCALL_H
#include "error_code.h"
int Topic_create_SC(char name[])
{
	message m;
	strcpy(m.m3_ca1,name);
//	printf("\nM new one %s",m.m3_ca1);
	return ( _syscall(PM_PROC_NR, TOPIC_CREATE, &m) );
}


int Topic_publisher_SC(char name[])
{
	int a;
	message m;
	a=getpid();
	strcpy(m.m3_ca1,name);
	m.m1_i1=a;
	return ( _syscall(PM_PROC_NR,TOPIC_PUBLISHER, &m) );
}

int Topic_subsriber_SC(char name[])
{
	int a;
	message m;
	a=getpid();
	strcpy(m.m3_ca1,name);
	m.m1_i1=a;
	return ( _syscall(PM_PROC_NR,TOPIC_SUBSCRIBER, &m) );
}

int Topic_publish_msg_SC(char name[])
{
	int a;
	message m;
	a=getpid();
	strcpy(m.m3_ca1,name);
	m.m1_i1=a;
	return ( _syscall(PM_PROC_NR,TOPIC_PUBLISH_MSG, &m) );
}


int Topic_Display_MSG_Table_SC(void)
{
	message m;
	return ( _syscall(PM_PROC_NR,TOPIC_DISPLAY_MSG_TABLE, &m) );
}

int Topic_init(void)
{
	message m;
	//printf("\nAbout to call init...!!");
	return ( _syscall(PM_PROC_NR, TOPIC_INIT, &m) );
}

int Topic_Receive_msg_SC(void)
{
	message m;
	m.m1_i1=getpid();
//	printf("\nAbout to recieve message...!!");

	return ( _syscall(PM_PROC_NR,TOPIC_RECEIVE_MSG, &m) );
}

int Topic_Display_subscriber(void)
{
	message m;
	return ( _syscall(PM_PROC_NR,TOPIC_DISPLAY_SUB, &m) );
}

int Topic_Display_publisher(void)
{
	message m;
	return ( _syscall(PM_PROC_NR,TOPIC_DISPLAY_PUB, &m) );
}

int Topic_Display_Topic(void)
{
	message m;
	return ( _syscall(PM_PROC_NR,TOPIC_DISPLAY_TOPIC, &m) );
}


extern int addNewMessage(int);

extern void init();
extern int Topic_create(char[]);
extern int get_TOPIC_ID(char[]);
extern int Topic_publish(char[],pid_t);
extern int Topic_subsciber(char[],pid_t);
extern int publish_message(char[],pid_t);

extern int get_message(char*,pid_t);
extern int process_msgid(int,char*,pid_t);

extern void Topic_lookup();
extern void show_subscribe();
extern void show_publish();
extern void show_message_topic();
#endif
