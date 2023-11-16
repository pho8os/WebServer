NAME		=	WebServ
CC			=	c++
FLAGS		=	-Wall -Wextra -Werror  -std=c++98 
OBJDIR 		=	.obj
HEADER		=	src/Server/Server.hpp \
				src/post/post.hpp
FILES		= 	src/Server/Server.cpp \
				src/post/post.cpp \
				src/post/main_post.cpp \
				src/main.cpp

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

re: fclean all
.PHONY: all clean fclean re