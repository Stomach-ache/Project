#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "update_and_cal_min.c"

#define MAXDATASIZE 1024

#define SERVERPORT 8000
#define MAXCONN_NUM 10
#define DEBUGE 1
#define KEY "request"



void doprocessing(int sock, char* client_ip);

int main(void)
{
	int sockfd, new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	/*  */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&client_addr, 0, sizeof(struct sockaddr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	/*  */
	if (bind
	    (sockfd, (struct sockaddr *) &server_addr,
	     sizeof(struct sockaddr)) == -1) {
		perror("bind error");
		exit(1);
	}
	/*  */
	if (listen(sockfd, MAXCONN_NUM) == -1) {
		perror("listen error");
		exit(1);
	}

	while (1) {
		sin_size = sizeof(struct sockaddr_in);
		/*  */
		if ((new_fd =
		     accept(sockfd, (struct sockaddr *) &client_addr,
			    &sin_size)) == -1) {
			perror("accept error");
			continue;
		}
#ifdef DEBUGE
	        printf("server: got connection from %s  port: %d\n",
		       inet_ntoa(client_addr.sin_addr),
		       client_addr.sin_port);
#endif
		/* Create child process */
		pid_t pid;
		pid = fork();
		if (pid < 0){
			perror("ERROR on fork");
			exit(1);
		}
		if (0 == pid){
			/* This is the child process */
			doprocessing(new_fd,
				     inet_ntoa(client_addr.sin_addr));
			close(new_fd);
			exit(0); 
		}
		else{
			close(new_fd);
		}
	}

	return 0;
}

void doprocessing(int sock, char* client_ip ){
	int numbytes;
	char buf[MAXDATASIZE];

	memset(buf, 0, sizeof(buf));

	if ((numbytes = recv(sock, buf, MAXDATASIZE, 0)) == -1) {
		perror("ERROR receiving form socket");
		exit(1);
	}
	if (numbytes) {
		buf[numbytes] = '\0';
#ifdef DEBUGE
	        printf("Here is the message: %s\n", buf); 
#endif

		char ans[256];
		strcpy(ans, update(buf, client_ip));
#ifdef DEBUGE
		printf("the answer is %s\n", ans);
#endif
		struct sockaddr_in result;
		result.sin_addr.s_addr = inet_addr(ans);
		if (0 == strcmp(KEY, buf))
				if(-1 == send(sock, &result, sizeof(result), 0)) {
						perror("ERROR sending to socket");
						exit(1);
				}
	}
#ifdef DEBUGE
	if (send(sock, "hi~~", 5, 0) == -1) {
		perror("ERROR sending to socket");
		exit(1);
	}
#endif
	
	return ;
}
