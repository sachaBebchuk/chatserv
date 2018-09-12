#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LEN 500
#define MAX_BUFFER_SIZE 1024
#define MAX_LOGIN_ATTEMPTS 5

struct message_buffer{
	int  size;
	char *buffer;
};

struct chat_response{
	int    size;
	time_t time;
	char   *username;
	char   *c_str;
};

struct login_response{
	int error;
	int countdown;
};

struct message_buffer* get_chat_response_buffer(struct chat_response*);
void                   free_message_buffer(struct message_buffer*);

#endif