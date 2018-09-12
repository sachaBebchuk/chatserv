#include "client_list.h"

struct client* add_client(int client_socket,struct client_list *cl){

	struct client      *client;
	struct client_node *cn;

	client = malloc(sizeof(struct client));
	cn = malloc(sizeof(struct client_node));

	client->s = client_socket;
	client->name = NULL;
	client->thread = 0;

	cn->client = client;
	cn->prev = cl->last;
	cn->next = NULL;

	if(cl->count){
		cl->last->next = cn;
		cl->last = cn;
		cl->count++;
	}else{
		cl->first = cn;
		cl->last = cn;
		cl->count++;
	}

	return client;
}

int remove_client(struct client *client, struct client_list *cl){

	struct client_node *ip = cl->first;

	while(ip && ip->client != client){
		ip = ip->next;
	}

	if(!ip){
		return -1;
	}

	if(ip->prev){
		ip->prev->next = ip->next;
	}

	if(ip->next){
		ip->next->prev = ip->prev;
	}

	destroy_client(client);
	free(ip);

	return 0;
}

struct client_list* init_client_list(struct client_list *cl){

	cl->first = NULL;
	cl->last  = NULL;
	cl->count = 0;

	return cl;
}

void destroy_client_list(struct client_list *cl){

	struct client_node *ip = cl->first;
	struct client      *client;

	while(ip){

		client = ip->client;

		pthread_cancel(client->thread);
		pthread_join(client->thread,NULL);

		destroy_client(client);

		if(!ip->next){
			free(ip);
			ip = NULL;
		}else{
			ip = ip->next;
			free(ip->prev);
		}
	}

	init_client_list(cl);
}

void destroy_client(struct client *client){

	close(client->s);
	
	if(client->name){
		free(client->name);
	}

	free(client);
}