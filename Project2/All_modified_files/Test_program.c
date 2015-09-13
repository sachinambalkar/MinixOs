#include <mysyscalllib.h>
#include<stdio.h>
#include<unistd.h>
#include<newSystemcall.h>
#include"error_code.h"
#include<stdlib.h>

/*
 *
#define MAXIMUM 100

#define FAILED -99
#define SUCCESS 100
#define ERROR_NOTFOUND -1
#define NO_SUBSCRIBED_PROCESS -2
#define NO_MESSAGE_AVAILABLE -4
#define TOPIC_ALREADY_EXIST -5
#define TOPIC_DOES_NOT_EXIST -6
#define NOT_VALID_PUBLISHER -7
 *
 */
int main(void)
{
int operationSelectedNumber,a,i,return_value;
char choice,NewTopicName[100],topicSelectedToPublish[100];
char topicSelectedToSubscribe[100],publishMessage[100];


do{
	system("clear");
	printf("\n@@@@@@@@@@@ Welcome @@@@@@@@@@@@@@@@");
	printf("\n1. Create new TOPIC (TopicCreate) ");
	printf("\n2. Show Available Topics (TopicLookup)");
	printf("\n3. Publisher Details");
	printf("\n4. Subscriber Details");
	printf("\n5. Register as Publisher (TopicPublisher)");
	printf("\n6. Register as Subscriber (TopicSubscriber)");
	printf("\n7. Publish Message (Publish)");
	printf("\n8. Receive Message (Retrieve)");
	printf("\n9. Clear Buffer");
	printf("\n Entered choice: ");
	scanf("%d",&operationSelectedNumber);

	switch(operationSelectedNumber)
	{
	case 1:
			printf("\nEnter Topic Name: ");
			scanf("%s",NewTopicName);
			if(Topic_create_SC(NewTopicName)==SUCCESS)
				printf("\nNew Topic is added...!!!\n");
			else
				printf("\nTopic Is Already Exist ...!!!\n");
			break;
	case 2:
			Topic_Display_Topic();
			break;
	case 3:
			Topic_Display_publisher();
			break;
	case 4:
			Topic_Display_subscriber();
			break;
	case 5:
			Topic_Display_Topic();
			printf("\nEnter Topic name to Publish: ");
			scanf("%s",topicSelectedToPublish);
			return_value=Topic_publisher_SC(topicSelectedToPublish);
			//printf("%d",return_value);
			if(return_value==SUCCESS)
				printf("\nSuccessfully registered as Publisher...!!!\n");
			else
				printf("\nTopic named ' %s ' Does not exist. Can't Published....!!!\n",topicSelectedToPublish);
			break;
	case 6:
			Topic_Display_Topic();
			printf("\nEnter Topic name to Subscribe: ");
			scanf("%s",topicSelectedToSubscribe);
			return_value=Topic_subsriber_SC(topicSelectedToSubscribe);
			if(return_value==SUCCESS)
				printf("\nSuccessfully registered as Subscriber...!!!\n");
			else
				printf("\nTopic named ' %s ' Does not exist. Can't Subscribed....!!!\n",topicSelectedToSubscribe);
			break;

	case 7:
		printf("\nEnter message to publish: ");
		scanf(" %s",publishMessage);
		Topic_publish_msg_SC(publishMessage);
		break;

	case 8:
		if(Topic_Receive_msg_SC()!=SUCCESS)
			printf("\nNo Message Available to read...!!!\n");
		break;

	case 9:Topic_init();
	printf("\nAll buffer data removed..!!!");
		break;

	default:
		printf("\nDefault Operation selected");
	}


	printf("\n\n@@@@@@@@@@ Continue(y/n) :");
	scanf(" %c",&choice);
}while(choice=='y');
return 0;

}
