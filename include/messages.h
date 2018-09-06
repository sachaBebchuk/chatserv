#ifndef MESSAGES_H

#define MESSAGES_H
#define MAX_MESSAGE_LEN 500
#define MAX_LOGIN_ATTEMPTS 5

struct login_request{
	int  size;
	char *name;
};

struct login_response{
	int error;
	int countdown;
};

struct message_request{
	int  size;
	char *m_str;
};

struct message_response{
	int    size;
	time_t time;
	int    name_size;
	char   *username;
	char   *m_str;
};

#endif