#include<stdio.h>
#include"jsonrpc.h"
#include<jansson.h>


json_t * test(jsonrpc_connection *conn,json_t* temp){
	return NULL;
}

int echo(jsonrpc_connection *conn,json_t* temp){
	jsonrpc_send_response(conn->client_fd,RESPONSE_TYPE_SUCESS,temp,conn->id);
	return 0;
}

int hello (jsonrpc_connection *conn,json_t* temp){
	//json_t *te = json_pack("{s:s s:s s:s}","jsonrpc","2.0","result","sucess","id",conn->id);
	jsonrpc_send_response(conn->client_fd,RESPONSE_TYPE_SUCESS,temp,conn->id);

}

void print_usage(){
	printf("Usage:\n");
	printf("\ts: starts server\n");
	printf("\tk: kills server\n");
}
int main(){
	int command;
	jsonrpc_register_method("echo",&echo);
	jsonrpc_register_method("hello",&hello);
	print_usage();
	while(command = getchar()){
		switch(command){
			case 's': jsonrpc_server_start(2001);
				  break;
			case 'k': return jsonrpc_server_stop();		
		}
		printf("\n:");
	}
}
