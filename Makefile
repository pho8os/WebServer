NAME = server

CPP = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address

SRC = Src/Server/Server.cpp Src/Request/Request.cpp Src/Response/Response.cpp Src/ConfigFile/ConfigFile.cpp Src/main.cpp

OBJ = $(SRC:.cpp=.o)

WHITE = \033[1;37m
GREEN = \033[1;32m
YELLOW = \033[1;33m

HEADER = Src/Server/Server.hpp Src/Request/Request.hpp Src/Response/Response.hpp Src/ConfigFile/ConfigFile.hpp

all : $(NAME)

$(NAME) : $(OBJ) $(HEADER)
	@echo "${YELLOW}SUCCESS ! ${END}"
	@$(CPP) $(CFLAGS) $(OBJ) -o $(NAME)

%.o : %.cpp $(HEADER)
	$(CPP) $(CFLAGS) -c $< -o $@
	@echo "${WHITE}LINKING ${END}"

clean :
	@echo "${WHITE}removing object files ${END}"
	@rm -rf $(OBJ)

fclean : clean
	@echo "${WHITE}removing executable ${END}"
	@rm -rf $(NAME)

re : fclean all

.PHONY : clean fclean re