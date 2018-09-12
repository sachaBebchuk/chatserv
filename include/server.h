#ifndef SERVER_H__
#define SERVER_H__

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
#include <time.h>

#include "messages.h"
#include "client_list.h"

//defines
#define DEFAULT_PORT 23456
#define MAX_CLIENTS  8
#define SERVER_NAME  "SERVER"

//functions
in_port_t get_port(char*);
int       init_server(in_port_t);
void      destroy_server();
void      server_loop();
void      handle_connection(int);
void      broadcast_chat(struct chat_response*);
void      broadcast_server_chat(char*);
void      send_message(struct client*,struct message_buffer*);
int       recv_message(struct client*,struct message_buffer*,int);
int       discard_socket_data(int,int);

//globals
struct client_list *clients;
pthread_mutex_t    *clients_mutex;
int                server_sock;
char               *server_name;

#endif