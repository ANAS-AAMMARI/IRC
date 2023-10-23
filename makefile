NAME=IRC

CPP = c++

CPPFLAGS=-Wall -Wextra -Werror -std=c++98

HEADER=	command.hpp \
		server.hpp 	\
		client.hpp	\
		tools.hpp	\
		channel.hpp	\
		mode_opt.hpp

SRC=	main.cpp \
		command.cpp \
		server.cpp	\
		client.cpp	\
		channel.cpp	\
		mode_opt.cpp

OBJ=$(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
