NAME = Webserv

SRC =	main.cpp \
Server.cpp

HPP = Server.hpp \
config.hpp

OBJ = $(SRC:.cpp=.o)

CPP = c++
CFLAGS = -Wall -Werror -Wextra
CVERSION = -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(CFLAGS) $(CVERSION) $(OBJ) -o $(NAME)

%.o: %.cpp $(HPP)
	$(CPP) $(CFLAGS) $(CVERSION) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re clean fclean