#include <iostream>
#include "md.hpp"
#include "Tintin_reporter.hpp"

#include <sys/socket.h>
#include <stdlib.h>

int	islock()
{
	if( access( LOCK_PATH, F_OK ) != -1 ) {
		return (1);
	} else {
		return (0);
	}
}

void lock()
{
	int fd;
	fd = open(LOCK_PATH, O_RDWR|O_CREAT, 0777);
	if (fd != -1)
		close(fd);
}

void unlock()
{
	if (unlink(LOCK_PATH) != 0)
	{
		perror("unlink()");
	}
}

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
			tss->env->report.Logstd("User disconnect !", INFO);
			run = 0;
			break;
		}
		if (ret > 0)
		{
			buf[ret - 1] = '\0';
			if (strncmp(buf, "quit", 4) == 0)
			{
				tss->env->report.Logstd("Quit ask ! Exit now !", INFO);
				pthread_mutex_lock(&tss->env->mutex);
				tss->env->exit = 0;
				close(tss->env->sock);
				pthread_mutex_unlock(&tss->env->mutex);
				run = 0;
				break;
			}
		}
		tss->env->report.Log(buf, LOG);
		bzero(buf, 100);
	}
	
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
	t_env	e;
	pthread_t	thread[3];
	e.connected = 0;
	e.exit = 1;
	e.report.Logstd("Started", INFO);
	if (islock())
	{
		
		e.report.Logstd("Already Start .lock exist", ERROR);
		return (0);
	}
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
	if (bind(e.sock, (struct sockaddr *)&e.sin, sizeof(e.sin)) == -1)
	{
		e.report.Logstd("Port already in use !", ERROR);
		unlock();
		return (1);
	}
	if (listen(e.sock, 5) == -1)
	{
		perror("listen()");
		return (1);
	}
	lock();
	e.report.Logstd("Daemon Ready !", INFO);
	while (e.exit)
	{
		t_sinsock *tss;
		tss = (t_sinsock*)malloc(sizeof(t_sinsock));
		tss->csinsize = sizeof(tss->csin);
		tss->csock = accept(e.sock, (struct sockaddr*)&tss->csin, &tss->csinsize);
		if (tss->csock == -1)
		{
			break;
		}
		else
		{
			tss->env = &e;

			std::string newcon("Connection detected ! FROM : ");
			newcon.append(inet_ntoa(tss->csin.sin_addr));
			e.report.Logstd(newcon , INFO);

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
					e.report.Logstd("Max connected ! reject ", INFO);
					char str[]  = "Limit connection";
					write(tss->csock, str, strlen(str));
					close(tss->csock);
					free(tss);
				}
		}
	}
	
	e.report.Logstd(" Exit Program ", INFO);
	unlock();
	return (0);
}

int	main(int argc, char **argv)
{
//	pid_t pID = fork();
//	if (pID == 0)
	{
		return (main2(argc, argv));
	}
//	return (0);
}
