#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>

int main(){
	int sock_fd,res,i=0;
	struct sockaddr_in client_sock;
	
	sock_fd = socket(AF_INET,SOCK_STREAM,0);
	if(sock_fd < 0){
		perror("Socket creation");
		return -1;
	}

	client_sock.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_sock.sin_port = 2001;
	client_sock.sin_family = AF_INET;
	
	res = connect(sock_fd,(struct sockaddr*)&client_sock,sizeof(struct sockaddr_in));
	if(res < 0){
		perror("Connecting");
		return -1;
	}
	
	char message[10],recieved[1024];
	for(i=0;i<5;i++){
		printf("Echo message: ");
		scanf("%s",message);
		send(sock_fd,message,10,0);
		perror("Write");
		printf("you wrote: %si\n",message);
		read(sock_fd,recieved,1024);
		perror("Read");
		printf("Server sent: %s\n",recieved);
	}
	close(sock_fd);
	perror("closing sock fd client");
}
