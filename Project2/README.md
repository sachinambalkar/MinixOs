The MINIX IPCs do not allow a user process (thread) to send a message to another. In this project, designed and implemented a set of system calls that will allow user processes to send and receive messages to one another and with additional functionalities.

Following system calls implemented for this purpose:

i.TopicLookup: that will allow a process to discover what interest groups are there.

ii.TopicCreate: that will allow a process to create an interest group.

iii.TopicPublisher: that will allow a process to declare itself a publisher of a specific interest group.

iv.TopicSubscriber: that will allow a process to declare itself a subscriber to an interest group.

v.Publish: that will allow a publisher to send a message to an interest group.

vi.Retrieve: that will allow a subscriber to retrieve one message from an interest group.

While a publisher is sending to a topic, no other user can publish to or retrieve from the topic. However, multiple subscribers can be retrieving messages from the same topic at the same time. Here maintained a buffer that can contain 5 messages for each topic. No publisher can send to the topic when the buffer is full.
