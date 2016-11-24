#ifndef JSONRPC_H
#define JSONRPC_H

#include<jansson.h>
typedef enum _bool{
	false,
	true
}bool;

typedef struct{
	int client_fd;
	bool connection_status;
}jsonrpc_connection;


int jsonrpc_server_start(int port);
int jsonrpc_server_stop();

typedef json_t* (*jsonrpc_method)(jsonrpc_connection*,json_t*);

int jsonrpc_register_method(char*,jsonrpc_method);


int jsonrpc_client_init(char*,int);
json_t* jsonrpc_client_invoke(char*,json_t*);

#endif
