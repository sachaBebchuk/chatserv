#include "server.h"
#include "client_thread.h"

void* client_thread(void * args){

	struct client *client = (struct client *) args;

	if(!client_login(client)){
		
		login_message(client);

		client_loop(client);
	}

	return NULL;
}

int client_login(struct client* client){

	char                  continue_loop = 1;
	char                  buffer[MAX_NAME_LEN+1];
	struct message_buffer mb;
	int                   recv_ret;
	struct login_response res;

	mb.buffer = buffer;

	res.countdown = MAX_LOGIN_ATTEMPTS;

	do{

		if(recv_message(client,&mb,MAX_NAME_LEN)){
			return 1;
		}

		res.error = check_client_name(buffer,clients);

		if(send(client->s,&res,sizeof(res),0) < 0){
			return 1;
		}

		if(res.error){
			res.countdown--;
			continue;
		}

		client->name = malloc( mb.size + 1);
		strncpy(client->name,buffer,mb.size);

		continue_loop = 0;

	}while(res.countdown && continue_loop);

	return !res.countdown;
}

void client_loop(struct client *client){
	
	char                  char_buffer[MAX_MESSAGE_LEN];
	struct message_buffer mb;
	struct chat_response  chat_response;

	chat_response.username = client->name;
	chat_response.c_str    = char_buffer;

	broadcast_chat(&chat_response);

	while(1){
		
		if(recv_message(client,&mb,MAX_MESSAGE_LEN)){
			break;
		}

		chat_response.time = time(NULL);

		broadcast_chat(&chat_response);

	};
}

int check_client_name(char *name, struct client_list *cl){

	int                result = 0;
	struct client_node *ip;

	if(!strcmp(server_name,name)){
		return 1;
	}

	pthread_mutex_lock(clients_mutex);

	for(ip = cl->first; ip; ip = ip->next){

		if(!ip->client->name)
			continue;

		if(!strcmp(ip->client->name,name)){
			result = 1;
			break;
		}
	}

	pthread_mutex_unlock(clients_mutex);

	return result;
}

void login_message(struct client* client){

	char message[MAX_MESSAGE_LEN];

	memset(message,0,MAX_MESSAGE_LEN);
	strcpy(message,client->name);
	strcat(message," is now online!");

	broadcast_server_chat(message);
}