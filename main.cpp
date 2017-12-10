#include <iostream>
#include "md.hpp"
#include "Tintin_reporter.hpp"
#include <sstream>
t_env *ge;

void sighandler(int signo)
{
	printf("[%d] \n", signo);
	std::stringstream ss;
	ss << "Signal Catch : [" << signo << "]";
	
	ge->report.Logstd(ss.str(), INFO);
	if (signo == SIGTERM || signo == SIGINT)
	{
		//done = 1;
	}
}

int main2(int argc, char **argv)
{
	t_env		e;
	struct		sigaction act;
	pthread_t	thread[3];

	ge = &e;
	e.connected = 0;
	e.exit = 1;
	e.report.Logstd("Started", INFO);

	printf("pid:%d \n", getpid());

	sigemptyset( &act.sa_mask );
	act.sa_flags = 0;
	act.sa_handler = sighandler;

	for(int u = 0; u < 64; u++)
	{
		sigaction(u,  &act, 0);
	}
	

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
		e.report.Logstd("While start ", INFO);

		t_sinsock *tss;
		tss = (t_sinsock*)malloc(sizeof(t_sinsock));
		tss->csinsize = sizeof(tss->csin);
		tss->csock = accept(e.sock, (struct sockaddr*)&tss->csin, &tss->csinsize);
		printf("accept return bitch\n");
		if (tss->csock < 0)
		{
			if (errno == EINTR)
				printf("Accept interrupt ! \n");
			continue;
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
	close(e.sock);

	e.report.Logstd(" Exit Program ", INFO);
	unlock();
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
