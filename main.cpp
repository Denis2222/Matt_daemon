#include <iostream>
#include "md.hpp"
#include "Tintin_reporter.hpp"

#include <sys/socket.h>
#include <stdlib.h>

void	mySignal( int sig ){
	printf("Signal SIGKILL");
}


void *thread_client(void *sin_sock)
{
	t_sin_sock	*tss;
	int			run;

	run = 1;
	tss = (t_sin_sock*)sin_sock;
	while (run)
	{
		sleep(2);
		std::cout << "I sleep for client call !" << tss->csock  << std::endl;
	}
	close(tss->csock);
	free(tss);
	return (NULL);
}


int main2(int argc, char **argv)
{
/*	std::cout << "Start main" << std::endl;
	std::string toto = "Yoloooo";
	Tintin_reporter report(toto);
	report.Hello();
*/
	int exit;
	int sock;
	pthread_t	thread[3];
	int connected = 0;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("socket():");
		return (1);
	}


	struct sockaddr_in sin;

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
	{
		perror("bind()");
		return (1);
	}
	if (listen(sock, 5) == -1)
	{
		perror("listen()");
		return (1);
	}
	
	exit = 1;
	while (exit)
	{
		std::cout << "Wait for connection !" << std::endl;
		t_sin_sock *tss;
		tss = (t_sin_sock*)malloc(sizeof(t_sin_sock));
		tss->csinsize = sizeof(tss->csin);
		tss->csock = accept(sock, (struct sockaddr*)&tss->csin, &tss->csinsize);
		if (tss->csock == -1)
		{
			perror("accept()");
			return (1);
		}
		else
		{
			std::cout << "New connection !" << tss->csock << std::endl;
			if (connected < 3)
			{
				pthread_create(&thread[connected], NULL, thread_client, (void *)tss);
				connected++;
			}
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	//pid_t pID = fork();
	//if (pID == 0)
	{
		return (main2(argc, argv));
	}
	//return (0);
}
