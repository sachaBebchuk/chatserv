#ifndef CLIENT_LIST_H__
#define CLIENT_LIST_H__

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//structs
struct client{
	char      *name;
	int       s;
	pthread_t thread;
};

struct client_node{
	struct client      *client;
	struct client_node *next;
	struct client_node *prev;
};

struct client_list{
	struct client_node *first;
	struct client_node *last;
	int                count;
};

struct client*      add_client(int,struct client_list*);
struct client_list* init_client_list(struct client_list*);
void                destroy_client_list(struct client_list*);

#endif