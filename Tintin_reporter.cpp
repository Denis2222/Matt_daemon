#include "md.hpp"
#include "Tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter(std::string filename)
{
	std::cout << "generate reporter !" << std::endl;
	this->filename = filename;
	std::cout << this->filename << std::endl;
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter&reporter)
{

	*this = reporter;
	this->filename.push_back('r');
	std::cout << "new reporter from copy !" << std::endl;
	std::cout << this->filename << std::endl;
}

Tintin_reporter::~Tintin_reporter()
{
	std::cout << "Delete reporter !" << std::endl;
}

Tintin_reporter& Tintin_reporter::operator=(const Tintin_reporter&reporter)
{
	if (this != &reporter)
	{
		this->filename = reporter.filename;
	}
	return (*this);
}


void Tintin_reporter::Hello(void)
{
	std::cout << "Hello " << this->filename << std::endl;
}
