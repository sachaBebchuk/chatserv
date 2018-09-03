
struct client* add_client(int client_socket){

	struct client      *client;
	struct client_node *cn;

	client = malloc(sizeof(struct client));
	cn = malloc(sizeof(struct client_node));

	client->s = client_socket;
	client->name = NULL;
	client->thread = 0;

	cn->client = client;
	cn->prev = clients->last;
	cn->next = NULL;

	clients->last->next = cn;
	clients->last = cn;
	clients->count++;

	pthread_mutex_unlock(clients_mutex);

	return client;
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