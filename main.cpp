#include <iostream>
#include "md.hpp"
#include "Tintin_reporter.hpp"

#include <sys/socket.h>
#include <stdlib.h>

void	mySignal( int sig )
{
	printf("Signal SIGKILL");
}


void *thread_client(void *sin_sock)
{
	t_sinsock	*tss;
	int			run;
	int			ret;

	run = 1;
	tss = (t_sinsock*)sin_sock;
	char *buf;

	buf = (char *)malloc(100);
	bzero(buf, 100);
	while (run)
	{
		ret = read(tss->csock, (void *)buf, 100);
		if (ret == 0)
		{
			std::cout << "Disconnect ! " << tss->csock  << std::endl;
			return (0);
		}
		buf[ret] = '\0';
		std::cout << "Incoming message : " << buf << std::endl;
		bzero(buf, 100);
	}
	close(tss->csock);
	free(tss);
	return (NULL);
}

void	newsinsock()
{
	
}

int main2(int argc, char **argv)
{
/*	
	std::cout << "Start main" << std::endl;
	std::string toto = "Yoloooo";
	Tintin_reporter report(toto);
	report.Hello();
*/
	int sock;

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

	int connected = 0;
	int exit;
	exit = 1;
	pthread_t	thread[3];
	while (exit)
	{
		std::cout << "Wait for connection !" << std::endl;
		t_sinsock *tss;
		tss = (t_sinsock*)malloc(sizeof(t_sinsock));
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
	pid_t pID = fork();
	if (pID == 0)
	{
		return (main2(argc, argv));
	}
	return (0);
}
