#include "server.h"

int main( int argc, char ** argv){
	
	in_port_t port;
	int       err;

	clients = NULL;
	clients_mutex = NULL;
	server_sock = 0;

	atexit(destroy_server);

	port = get_port( argc > 1 ? argv[1]: NULL );

	err = init_server(port);

	if(err){
		printf("Couldn't initialize server, exiting.\n");
		exit(1);
	}

	server_loop();

	exit(0);
}

int init_server(in_port_t port){

	int				   err;
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = port;

	server_sock = socket(AF_INET,SOCK_STREAM,0);

	if(server_sock < 0){
		perror("socket");
		return -1;
	}

	err = bind(server_sock, (struct sockaddr *)&addr, sizeof(addr));

	if(err){
		perror("bind");
		return -1;
	}

	err = listen(server_sock, MAX_CLIENTS);

	if(err){
		perror("listen");
		return -1;
	}

	printf("The server is now bound and listening to all the interfaces in port %d\n"
		   "Awaiting connections...\n",ntohs(port));
	fflush(stdout);

	clients_mutex = malloc(sizeof(pthread_mutex_t));
	err = pthread_mutex_init(clients_mutex,NULL);
	if(err){
		perror("mutex");
		return -1;
	}

	clients = malloc(sizeof(struct client_list));
	init_client_list(clients);

	return 0;
}

void destroy_server(){

	if(server_sock){
		close(server_sock);
	}
	
	if(clients_mutex){
		pthread_mutex_destroy(clients_mutex);	
		free(clients_mutex);
	}

	if(clients){
		destroy_client_list(clients);
		free(clients);
	}
}

in_port_t get_port(char *servname_str){

	in_port_t      port;
	struct servent *serv;

	if(!servname_str){
		return htons(DEFAULT_PORT);		
	}
	
	if(port = atoi(servname_str)){
		return htons(port);
	}

	serv = getservbyname(servname_str,NULL);
	endservent();

	if(serv){
		return serv->s_port;
	}

	printf("Incorrect service or port, using default port.\n");
	fflush(stdout);

	return htons(DEFAULT_PORT);
}

void server_loop(){

	int  accepted_socket;

	while(1){

		if( pthread_mutex_trylock(clients_mutex) || clients->count == MAX_CLIENTS){
			pthread_mutex_unlock(clients_mutex);
			sleep(1);
			continue;
		}

		accepted_socket = accept(server_sock,NULL,NULL);

		handle_connection(accepted_socket);
	}
}

void handle_connection(int client_socket){
	
}

void broadcast_message(struct message_response *mr){

	struct client_node *iterator_p = clients->first;

	while(iterator_p){

		if(!iterator_p->client->name){
			continue;
		}

		send_message(iterator_p->client,mr);

		iterator_p = iterator_p->next;
	}
}

void send_message( struct client *client, struct message_response *mr){

	ssize_t b_sent = 0;
	ssize_t send_result;
	
	while( b_sent < mr->m_size ){

		send_result = send(client->s, mr, mr->m_size, 0);

		if( send_result == -1){
			return;
		}

		b_sent += send_result;
	}
}

struct client_list* init_client_list(struct client_list *cl){

	cl->first = NULL;
	cl->last  = NULL;
	cl->count = 0;

	return cl;
}

void destroy_client_list(struct client_list *cl){

	struct client_node *iterator_p = cl->first;
	struct client      *client;

	while(iterator_p){

		client = iterator_p->client;

		pthread_cancel(client->thread);
		close(client->s);
		free(client->name);
		free(client);

		if(!iterator_p->next){
			free(iterator_p);
			iterator_p = NULL;
		}else{
			iterator_p = iterator_p->next;
			free(iterator_p->prev);
		}
	}

	init_client_list(cl);
}