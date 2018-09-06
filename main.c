#include "server.h"

int main( int argc, char ** argv){
	
	in_port_t port;
	int       err;

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