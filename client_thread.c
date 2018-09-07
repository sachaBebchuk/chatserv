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
	int                   name_size;
	int                   recv_ret;
	struct login_response res;

	res.countdown = MAX_LOGIN_ATTEMPTS;

	do{

		//Read metadata
		if(recv(client->s,&name_size,sizeof(name_size),MSG_WAITALL) < 0){
			return 1;
		}

		name_size = name_size > MAX_NAME_LEN ? MAX_NAME_LEN : name_size;

		memset(buffer,0,sizeof(buffer));

		//Read login name
		if(recv(client->s,buffer,name_size,MSG_WAITALL) < 0){
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

		client->name = malloc( name_size + 1);
		strncpy(client->name,buffer,name_size);

		continue_loop = 0;

	}while(res.countdown && continue_loop);

	return !res.countdown;
}

void client_loop(struct client* client){
	
	char continue_loop = 0;

	do{

	}while(continue_loop);
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

	memset(message,0,'\0');
	strcpy(message,client->name);
	strcat(message," is now online!");

	broadcast_server_chat(message);
}