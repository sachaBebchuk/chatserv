#ifndef SERVER_H__

//defines
#define SERVER_H__
#define DEFAULT_PORT 23456
#define MAX_CLIENTS  8
#define MAX_NAME_LEN 64

//Includes
//we must use THREADING!!!
#include <pthread.h>
//You know what these are for
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
//Protocol stuff
#include "messages.h"

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

//functions
int                 init_server(in_port_t);
void                destroy_server();
in_port_t           get_port(char*);
void                server_loop();
void                handle_connection(int);
void*               client_thread(void*);
struct client*      add_client(int);
void                broadcast_message(struct message_response*);
void                send_message(struct client*,struct message_response*);
struct client_list* init_client_list(struct client_list*);
void                destroy_client_list(struct client_list*);

//globals
struct client_list *clients;
pthread_mutex_t    *clients_mutex;
int                server_sock;

#endif