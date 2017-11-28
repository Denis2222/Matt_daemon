NAME	= matt_daemon
SRC		=	main.cpp \
			Tintin_reporter.cpp

OBJ			= $(SRC:.cpp=.o)
CC			= clang++ -Wall -Wextra -Werror

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)
