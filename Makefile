NAME	= matt_daemon
SRC		=	main.cpp \
			lock.cpp \
			thread.cpp \
			Tintin_reporter.cpp

OBJ			= $(SRC:.cpp=.o)
CC			= g++ -lpthread -std=c++11

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)
