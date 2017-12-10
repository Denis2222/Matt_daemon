#include "md.hpp"

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
				kill(getpid(),SIGUSR1);
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


