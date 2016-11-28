#include<stdio.h>
#include"jsonrpc.h"
#include<jansson.h>


json_t * test(jsonrpc_connection *conn,json_t* temp){
	return NULL;
}


int mining_authorize(jsonrpc_connection *conn,json_t* temp){
	//Check if temp is a json array and has two members otherwise throw invalid params exception
	char *username = json_string_value(json_array_get(temp,0));
	char *password = json_string_value(json_array_get(temp,1));
	
	//do auth
	/* Psuedocode
	if(auth is sucessful){
		json_t *auth_okay = json_pack("{s:s}","result","sucess");
		jsonrpc_send_response(conn->client_fd,RESPONSE_TYPE_SUCESS,auth_fail,conn->id);
		return 0;
	}else{
		json_t *auth_fail = json_pack("{s:s}","error","{"message":"auth failed","code":24,"data":"null"}");
		jsonrpc_send_response(conn->client_fd,RESPONSE_TYPE_FAILURE,auth_fail,conn->id);	
		return -1;
	}
	*/
}

int mining_subscribe (jsonrpc_connection *conn,json_t* temp){
	//Psuedocode
	/*
	check if already authorized
	* add subscription for set_difficulty and notify
	* construct params using subscriptions,extra nonce and length of extra nonce
		json_t *sub_okay = json_pack("{s:s s:s s:s}","id",conn->id,"result",<constructed_params>,"error","null");
		


	*/	
	jsonrpc_send_response(conn->client_fd,RESPONSE_TYPE_SUCESS,sub_okay,conn->id);

}

void print_usage(){
	printf("Usage:\n");
	printf("\ts: starts server\n");
	printf("\tk: kills server\n");
}
int main(){
	int command;
	jsonrpc_register_method("mining.authorize",&mining_authorize);
	jsonrpc_register_method("mining.subscribe",&mining_subscribe);
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
