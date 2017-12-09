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

	pthread_mutex_lock(&tss->env->mutex);
	tss->env->connected++;
	tss->env->connect[tss->nb] = 1;
	pthread_mutex_unlock(&tss->env->mutex);


	buf = (char *)malloc(100);
	bzero(buf, 100);
	while (run)
	{
		ret = read(tss->csock, (void *)buf, 100);
		if (ret == 0)
		{
			run = 0;
			break;
		}
		buf[ret] = '\0';
		std::cout << "Incoming message : " << buf << std::endl;
		if (strncmp(buf, "deco", 4) == 0)
		{
			run = 0;
			break;
		}
		bzero(buf, 100);
	}
	
	std::cout << "Disconnect ! " << tss->csock  << std::endl;
	pthread_mutex_lock(&tss->env->mutex);
	tss->env->connected--;
	tss->env->connect[tss->nb] = 0;
	pthread_mutex_unlock(&tss->env->mutex);

	close(tss->csock);
	free(tss);
	return (NULL);
}

int main2(int argc, char **argv)
{
/*	
	std::cout << "Start main" << std::endl;
	std::string toto = "Yoloooo";
	Tintin_reporter report(toto);
	report.Hello();
*/
	printf("Start !\n");

	t_env	e;
	pthread_t	thread[3];
	e.connected = 0;
	e.exit = 1;
	e.sock = socket(AF_INET, SOCK_STREAM, 0);
	e.connect[0] = 0;
	e.connect[1] = 0;
	e.connect[2] = 0;
	pthread_mutex_init(&e.mutex, NULL);
	if (e.sock == -1)
	{
		perror("socket():");
		return (1);
	}
	e.sin.sin_addr.s_addr = htonl(INADDR_ANY);
	e.sin.sin_family = AF_INET;
	e.sin.sin_port = htons(PORT);
	printf("Bind !");
	if (bind(e.sock, (struct sockaddr *)&e.sin, sizeof(e.sin)) == -1)
	{
		perror("bind()");
		return (1);
	}
	if (listen(e.sock, 5) == -1)
	{
		perror("listen()");
		return (1);
	}
	while (e.exit)
	{
		printf("Wait for connection ! connected : %d\n", e.connected);
		t_sinsock *tss;
		tss = (t_sinsock*)malloc(sizeof(t_sinsock));
		tss->csinsize = sizeof(tss->csin);
		tss->csock = accept(e.sock, (struct sockaddr*)&tss->csin, &tss->csinsize);
		if (tss->csock == -1)
		{
			perror("accept()");
			return (1);
		}
		else
		{
			tss->env = &e;
			std::cout << "New connection !" << tss->csock << std::endl;
				int can;
				int i;
				
				can = 0;
				i = 0;
				while (i < 3) {
					if (e.connect[i] == 0) {
						tss->thread = &thread[i];
						tss->nb = i;
						pthread_create(&thread[i], NULL, thread_client, (void *)tss);
						can = 1;
						i = 3;
					}
					i++;
				}
				if (!can)
				{
					printf("Max connected ! reject ");
					char str[]  = "Limit connection";
					write(tss->csock, str, strlen(str));
					close(tss->csock);
					free(tss);
				}
		}
	}
	
	printf("Exit program !\n");
	return (0);
}

int	main(int argc, char **argv)
{
//	pid_t pID = fork();
//	if (pID == 0)
	{
		printf("Start main !\n");
		return (main2(argc, argv));
	}
//	return (0);
}
