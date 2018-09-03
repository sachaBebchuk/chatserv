#ifndef MESSAGES_H

#define MESSAGES_H
#define MAX_MESSAGE_LEN 1024

struct login_request{
	int  m_size;
	char *name;
};

struct login_response{
	int m_size;
	int error;
	int countdown;
};

struct message_request{
	int  m_size;
	char *m_str;
};

struct message_response{
	int    m_size;
	time_t m_time;
	int    name_size;
	char   *username;
	char   *m_str;
};

#endif