#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

class Tintin_reporter
{
	public:
		Tintin_reporter(std::string filename);
		Tintin_reporter(const Tintin_reporter&reporter);
		~Tintin_reporter();
		Tintin_reporter& operator=(const Tintin_reporter&reporter);

		void Hello();

	private:
		std::string filename;

};

#endif
