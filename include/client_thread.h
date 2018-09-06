#ifndef CLIENT_THREAD_H__
#define CLIENT_THREAD_H__

#include "messages.h"

#define MAX_NAME_LEN 64
#define BUFFER_SIZE  MAX_MESSAGE_LEN

void*      client_thread(void*);
int        client_login(struct client*);
void       client_loop(struct client*);
int        check_client_name(char*,struct client_list*);

#endif