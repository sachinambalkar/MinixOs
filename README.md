# MinixOs

Project 1:
In this project, implemented new shell called Myshell. Myshell provides and interactive shell that lets you use the programs provided with the Minix system. Besides, Myshell can support the execution of list of commands in a order and also support integer variables. We implement a calculator application that can use the variables as input and output.

Project 2:
The MINIX IPCs do not allow a user process (thread) to send a message to another. In this project, designed and implemented a set of system calls that will allow user processes to send and receive messages to one another and with additional functionalities.
Following system calls implemented for this purpose:
       TopicLookup: that will allow a process to discover what interest groups are there.
       TopicCreate: that will allow a process to create an interest group.
       TopicPublisher: that will allow a process to declare itself a publisher of a specific interest group.
       TopicSubscriber: that will allow a process to declare itself a subscriber to an interest group.
       Publish: that will allow a publisher to send a message to an interest group.
       Retrieve: that will allow a subscriber to retrieve one message from an interest group.
While a publisher is sending to a topic, no other user can publish to or retrieve from the topic. However, multiple subscribers can be retrieving messages from the same topic at the same time. Here maintained a buffer that can contain 5 messages for each topic. No publisher can send to the topic when the buffer is full.

Project 3:
In this project, implemented one File System Tool.
It is capable to file the below problems of damage in File System:
1. The directory file is damaged
2. The inode of directory is damaged
3. The inode and the directory file are both damaged.
