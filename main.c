#include<stdio.h>
#include"jsonrpc.h"
#include<jansson.h>


json_t * test(jsonrpc_connection *conn,json_t* temp){

	printf("teststetetsdtsdgfsdfasdfasdfgasdjcbsdjbhcasduk");
	return NULL;
}


void print_usage(){
	printf("Usage:\n");
	printf("\ts: starts server\n");
	printf("\tk: kills server\n");
}
int main(){
	int command;
	jsonrpc_register_method("test",&test);
	print_usage();
	while(command = getchar()){
		switch(command){
			case 's': jsonrpc_server_start(8547);
				  break;
			case 'k': return jsonrpc_server_stop();		
		}
		printf("\nEnter:");
	}
}
