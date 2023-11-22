NAME		=	WebServ
CC			=	c++ -std=c++98 -fsanitize=address
FLAGS		=	-Wall -Wextra -Werror  
OBJDIR 		=	.obj

HEADER		=	Src/ConfigFile/ConfigFile.hpp \
				Src/ConfigFile/ConfigFile.hpp \
				Src/Request/post/post.hpp \
				Src/Request/Request.hpp \
				Src/Response/Response.hpp \
				Src/Server/Server.hpp \
				Src/cgi/Cgi.hpp \

FILES		= 	Src/Request/post/post.cpp \
				Src/Request/Request.cpp \
				Src/Response/Response.cpp \
				Src/ConfigFile/ConfigFile.cpp \
				Src/Server/Server.cpp \
				Src/main.cpp \
				Src/cgi/Cgi.cpp \

SRC			=	$(FILES:.cpp=.o)
OBJ			=	$(addprefix $(OBJDIR)/, $(SRC))

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(OBJ) -o $(NAME) 
	@echo "🧪 Server Ready!"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(OBJ)
	@echo  "🔥 Deleting OBJS."

fclean: clean
	@rm -rf  $(NAME)
	@echo  " 👾 Deleting WebServ"

install :
	# @chmod +x scripts/script.sh
	# scripts/script.sh

re: fclean all
.PHONY: all clean fclean re