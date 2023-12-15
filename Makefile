NAME		= WebServ
CC			= c++  -g -fsanitize=address 
FLAGS		= -Wall -Wextra -Werror  -std=c++98 -g
OBJDIR 		= .obj

FILES		= 	src/main \
				src/ConfigFile/ConfigFile \
				src/Server/Server \
				src/Request/Request \
				src/Response/Response \
				src/Cgi/Cgi \

HEADER		=	src/ConfigFile/ConfigFile.hpp \
				src/ConfigFile/ConfigFile.tpp  \
				src/Server/Server.hpp \
				src/Request/Request.hpp \
				src/Response/Response.cpp \
				src/Cgi/Cgi.hpp \

SRC			= $(FILES:=.cpp)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))


all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(FLAGS) $(OBJ)   -o $(NAME) 
	@echo "🧪 Server Ready!"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -g -c $< -o $@ 
	@echo "🧬 Serving!!"

clean:
	@rm -rf $(OBJDIR) $(OBJ)
	@echo  "🔥 Deleting OBJS."

fclean: clean
	@rm -rf  $(NAME)
	@echo  "👾 Deleting PmergeMe."

re: fclean all
.PHONY: all clean fclean re