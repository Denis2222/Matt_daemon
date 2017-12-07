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
#define MAX_CLIENT 3

typedef struct			s_sinsock
{
	int					csock;
	struct sockaddr_in	csin;
	socklen_t			csinsize;
	pthread_mutex_t		*mutex;
	pthread_t			thread;
}						t_sinsock;

typedef struct			s_env
{
	int					sock; //Server
	int					exit;
	struct sockaddr_in	sin;

	//Unsafe
	int					connected;
	pthread_mutex_t		*mutex;

	t_sinsock			client[MAX_CLIENT];
}						t_env;
