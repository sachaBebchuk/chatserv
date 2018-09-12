#include "messages.h"

struct message_buffer* get_chat_response_buffer(struct chat_response *cr){

	struct message_buffer *mb = malloc(sizeof(struct message_buffer));
	char                  *bp;
	int                   username_len = strlen(cr->username);
	int                   message_len = strlen(cr->c_str) ;

	mb->size = sizeof(cr->size) + sizeof(cr->time) + username_len + 1 + message_len;
	mb->buffer = malloc(mb->size);

	cr->size = mb->size;

	*((time_t*) mb->buffer) = cr->time;

	bp = mb->buffer + sizeof(time_t);
	strcat(bp,cr->username);
	strcat(bp,"\n");
	strcat(bp,cr->c_str);

	return mb;
}

void free_message_buffer( struct message_buffer *mb){

	free(mb->buffer);
	free(mb);
	
	return;
}