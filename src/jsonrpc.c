#include<jsonrpc.h>
#include<syslog.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<errno.h>
#include<poll.h>


#define MAX_CONNECTIONS 20
#define MAX_METHODS 10

bool server_status = false;
int server_port,server_socket_fd,connections;
pthread_t handlers[MAX_CONNECTIONS];
int client_socket_fd[MAX_CONNECTIONS];
jsonrpc_connection client_connections[MAX_CONNECTIONS];

int handler_threads_index = 0;
pthread_t jsonrpc_connection_acceptor_thread;
struct sockaddr_in server_socket,client_socket;


int jsonrpc_method_index = 0;
char *jsonrpc_methods_name[MAX_METHODS];
jsonrpc_method jsonrpc_methods[MAX_METHODS];

int jsonrpc_send_response(int fd,int response_type,json_t* result,char *id){
	json_t *response;  //= (json_t*)malloc(sizeof(json_t));
	if(response_type == RESPONSE_TYPE_SUCESS){
		response = json_pack("{s:s s:s s:s}","jsonrpc","2.0","result","pass",/*json_dumps(result,JSON_COMPACT),*/"id",id);
	}else if(response_type == RESPONSE_TYPE_FAILURE){
		response = json_pack("{s:s s:s s:s}","jsonrpc","2.0","error","fail",/*json_dumps(result,JSON_COMPACT),*/"id",id);
	}else{
		syslog(LOG_ERR,"wrong response type");
	}
	char *final_response = strcat(json_dumps(response,JSON_COMPACT),"\n");
	return send(fd,final_response,strlen(final_response),0);
}

int jsonrpc_register_method(char *method, int (*jsonrpc_method)(jsonrpc_connection*,json_t*)){
	if(jsonrpc_method_index < MAX_METHODS){
		jsonrpc_methods_name[jsonrpc_method_index] = method;
		jsonrpc_methods[jsonrpc_method_index] = jsonrpc_method;
		syslog(LOG_INFO,"added jsonrpc_method %s with %p",jsonrpc_methods_name[jsonrpc_method_index],jsonrpc_methods[jsonrpc_method_index]);
	}else{
		syslog(LOG_ERR,"max methods reached!");
	}
}

void jsonrpc_connection_handler(void * client_connection){
	int res = 0;
	//read
	//parse
	//invoke jsonrpc_mehtod
	//send_response
	//repeat
	jsonrpc_connection *conn = (jsonrpc_connection*)client_connection;
	int client_fd = (int)(conn->client_fd);
	char buffer[1024];
	struct pollfd *fds =(struct pollfd*) malloc(sizeof(struct pollfd));
	fds->fd = client_fd;
	fds->events = POLLIN;
	res = poll(fds,1,60*1000);
	while((fds->revents&POLLIN == POLLIN) && server_status && (res > 0)){
		res = recv(client_fd,buffer,1024,0);
		//parse message here and invoke jsonrpc method
		syslog(LOG_INFO,"Function pointer %p",jsonrpc_methods[jsonrpc_method_index]);
		//syslog(LOG_INFO,"Recieved from %d: %s",client_fd,buffer);
		jsonrpc_methods[jsonrpc_method_index](conn,NULL);
	}
}
void jsonrpc_connection_acceptor(){
	int res = 0;
	openlog("jsonrpc_connection_acceptor",LOG_PID|LOG_CONS,LOG_USER);
	handler_threads_index = 0;
	while(server_status){
		if(connections < MAX_CONNECTIONS){
			//server socket accept
			//syslog(LOG_INFO,"Accepting connections");
			int count = sizeof(struct sockaddr);
			res = accept(server_socket_fd,(struct sockaddr_in*)&client_socket,(socklen_t*)&count);
			if(res < 0){
				syslog(LOG_ERR,"accept failed");
				perror("accept");
				server_status = false;
				return server_status;
			}else{
				syslog(LOG_INFO,"Got a connection %s",inet_ntoa(client_socket.sin_addr));
				client_socket_fd[handler_threads_index] = res;
				
				jsonrpc_connection conn;
				conn.client_fd = res;
				client_connections[handler_threads_index] = conn;

				syslog(LOG_INFO,"creating jsonrpc_handler pthread");
				res = pthread_create(&handlers[handler_threads_index],NULL,jsonrpc_connection_handler,(void*)&client_connections[handler_threads_index]);
				if(res != 0){
					syslog(LOG_ERR,"connection handler threade create failed");
					perror("connection handler thread creation");
					server_status = false;
					return server_status;
				}
				//create jsonrpc_connection_handler pthread
				syslog(LOG_DEBUG,"Creating jsonrpc_connection_handler for %d fd",client_socket_fd[handler_threads_index]);
				handler_threads_index++;
			}
		}
	}
}
int jsonrpc_server_start(int port){
	int res = 0;
	server_port = port;
	jsonrpc_connection_acceptor_thread = (pthread_t*)malloc(sizeof(pthread_t));

	openlog("jsonrpc_server",LOG_PID|LOG_CONS,LOG_USER);
	
	//server socket creation
	server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
	if(server_socket_fd < 0){
		syslog(LOG_ERR,"server socket creation failed");		
		perror("creation");
		return server_status;
	}
	syslog(LOG_DEBUG,"server socket created");
	
	server_socket.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_socket.sin_port = port;
	server_socket.sin_family = AF_INET;
	
	//server socket binding
	res = bind(server_socket_fd,(struct sockaddr*)&server_socket,sizeof(struct sockaddr_in));
	if(res < 0){
		syslog(LOG_ERR,"server socket binding failed");
		perror("binding");
		return server_status;
	}
	syslog(LOG_DEBUG,"server socket binding succesful");
	
	//server socket listen
	res = listen(server_socket_fd,0);
	if(res < 0){
		syslog(LOG_ERR,"server socket listen failed");
		perror("listening");
		return server_status;
	}
	syslog(LOG_DEBUG,"server socket listening");
	
	//pthread_create()  for jsonrpc_connection_acceptor
	res = pthread_create(&jsonrpc_connection_acceptor_thread,NULL,jsonrpc_connection_acceptor,NULL);
	if(res != 0){
		syslog(LOG_ERR,"pthread create for connection acceptor failed");
		perror("connection acceptor pthread_create");
		return server_status;
	}
	server_status = true;

	return server_status;
}

int jsonrpc_server_stop(){
	int i,res = 0;
	server_status = false;
	
	//pthread_kill() for jsonrpc_connection_acceptor
	res = pthread_kill(jsonrpc_connection_acceptor,9);
	if(res != 0){
		syslog(LOG_ERR,"Couldnt kill jsonrpc_connection_acceptor thread");
	}
	
	//loop through all handlers and kill them
	for(i=0;i<handler_threads_index;i++){
		res = pthread_kill(handlers[i],9);
		if(res != 0){
			syslog(LOG_ERR,"couldnot kill handler thread at index %d",i);
		}
	}
	
	//loop thorugh all client socket fd and close them
	for(i=0;i<handler_threads_index;i++){
		res = close(client_socket_fd[i]);
		if(res < 0){
			syslog(LOG_ERR,"Couldnt close client socket fd at index %d",i);
		}
	}


	return server_status;
}
