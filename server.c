#include "server.h"
#include "client_thread.h"

int init_server(in_port_t port){

	int                err;
	struct sockaddr_in addr;

	server_name = SERVER_NAME;
	clients = NULL;
	clients_mutex = NULL;
	server_sock = 0;

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

	pthread_t     thread;
	struct client *client;

	pthread_mutex_lock(clients_mutex);

	client = add_client(client_socket,clients);

	pthread_create(&(client->thread),NULL,client_thread,client);

	pthread_mutex_unlock(clients_mutex);
}

void broadcast_chat(struct chat_response *cr){

	struct client_node    *ip = clients->first;
	struct message_buffer *mb = get_chat_response_buffer(cr);

	while(ip){

		if(!ip->client->name){
			continue;
		}

		send_message(ip->client,mb);

		ip = ip->next;
	}

	free_message_buffer(mb);
}

void send_message(struct client *client, struct message_buffer *mb){

	ssize_t b_sent = 0;
	ssize_t send_result;
	
	while( b_sent < mb->buffer_size ){

		send_result = send(client->s, mb->buffer, mb->buffer_size, 0);

		if( send_result == -1){
			return;
		}

		b_sent += send_result;
	}
}

void broadcast_server_chat(char *message){

	struct chat_response chat_response;

	chat_response.time = time(NULL);
	chat_response.username = server_name;
	chat_response.m_str = message;

	broadcast_chat(&chat_response);
}