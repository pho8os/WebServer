#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/26 21:30:28 by zmakhkha          #+#    #+#              #
#    Updated: 2023/10/26 21:30:29 by zmakhkha         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = serv

SRC =	src/Cgi.cpp \
		src/Response.cpp \
		src/Server.cpp \
		src/main.cpp

HPP =	src/all.hpp \
		src/Cgi.hpp \
		src/Response.hpp \
		src/Server.hpp

OBJ = $(SRC:.cpp=.o)

CPP = c++
CFLAGS = -Wall -Werror -Wextra
CVERSION = -std=c++98 -fsanitize=address

all:$(NAME)

$(NAME) : $(OBJ)
	$(CPP) $(CFLAGS) $(CVERSION) $(OBJ) -o $(NAME)

%.o:%.cpp $(HPP)
	$(CPP) $(CFLAGS) $(CVERSION) -c $< -o $@

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all re clean fclean