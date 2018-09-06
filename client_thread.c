#include "server.h"
#include "client_thread.h"

void* client_thread(void * args){

	struct client *client = (struct client *) args;

	if(!client_login(client)){
		//avisar el logeo
		client_loop(client);
	}

	return NULL;
}

int client_login(struct client* client){

	char                  continue_loop = 1;
	char                  buffer[MAX_NAME_LEN+1];
	struct login_request  req;
	struct login_response res;
	int                   recv_ret;

	res.countdown = MAX_LOGIN_ATTEMPTS;

	req.name = buffer;

	do{

		//Read metadata
		if(recv(client->s,&(req.m_size),sizeof(req.m_size),MSG_WAITALL) < 0){
			return 1;
		}

		memset(buffer,0,sizeof(buffer));

		//Read login name
		if(recv(client->s,req.name,(req.m_size - sizeof(req.m_size)),MSG_WAITALL) < 0){
			return 1;
		}

		res.error = check_client_name(req.name,clients);

		if(send(client->s,&res,sizeof(res),0) < 0){
			return 1;
		}

		if(res.error){
			res.countdown--;
			continue;
		}

		client->name = malloc( req.m_size - sizeof(req.m_size) + 1);
		strncpy(client->name,req.name,req.m_size);

		continue_loop = 0;

	}while(res.countdown && continue_loop);

	return !res.countdown;
}

void client_loop(struct client* client){
	
	char end_loop = 0;

	do{

	}while(end_loop);
}

int check_client_name(char *name, struct client_list *cl){

	int                result = 0;
	struct client_node *ip;

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