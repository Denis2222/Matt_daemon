#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4242

typedef struct			s_sin_sock
{
	int					csock;
	struct sockaddr_in	csin;
	socklen_t			csinsize;
}						t_sin_sock;
