#include <iostream>
#include "md.hpp"
#include "Tintin_reporter.hpp"

void	mySignal( int sig ){
	printf("Signal SIGKILL");
}


int main2(int argc, char **argv)
{
/*	std::cout << "Start main" << std::endl;
	std::string toto = "Yoloooo";
	Tintin_reporter report(toto);
	report.Hello();
*/
	int exit;

	exit = 1;

	while (exit)
	{
		sleep(2);
		std::cout << "I Live" << std::endl;
	}
}

int	main(int argc, char **argv)
{
	pid_t pID = fork();
	if (pID == 0)
	{
		main2(argc, argv);
	}
	return (0);
}
