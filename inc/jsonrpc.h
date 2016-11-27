#ifndef JSONRPC_H
#define JSONRPC_H

#include<jansson.h>

//constants
#define RESPONSE_TYPE_FAILURE 0
#define RESPONSE_TYPE_SUCESS 1


typedef enum _bool{
	false,
	true
}bool;

typedef struct{
	int client_fd;
	bool connection_status;
	char *id;
}jsonrpc_connection;


int jsonrpc_server_start(int port);
int jsonrpc_server_stop();
int jsonrpc_send_response(int,int,json_t*,char*);

typedef int (*jsonrpc_method)(jsonrpc_connection*,json_t*);

int jsonrpc_register_method(char*,jsonrpc_method);
void print_json_error(json_error_t*);

int jsonrpc_client_init(char*,int);
json_t* jsonrpc_client_invoke(char*,json_t*);

#endif
