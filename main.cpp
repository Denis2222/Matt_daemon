#include "md.hpp"

int	main(int argc, char **argv)
{
	pid_t pID = fork();

	if (pID == 0)
	{
		if (getuid() == 0)
			return (daemon(argc, argv));
		else
		{
			printf("Not Root !");
			return (0);
		}
	}
	return (0);
}
