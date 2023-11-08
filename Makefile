NAME		= WebServ
CC			= c++
FLAGS		= -Wall -Wextra -Werror  -std=c++98 
OBJDIR 		= .obj
FILES		=  Src/main  Src/ConfigFile/ConfigFile
SRC			= $(FILES:=.cpp)
OBJ			= $(addprefix $(OBJDIR)/, $(FILES:=.o))
HEADER		= Src/ConfigFile/ConfigFile.hpp Src/ConfigFile/ConfigFile.tpp


all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(OBJ) -o $(NAME) 
	@echo "🧪 Server Ready!"

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@ 
	@echo " 🧬 Serving!!"

clean:
	@rm -rf $(OBJDIR) $(OBJ)
	@echo  "🔥 Deleting OBJS."

fclean: clean
	@rm -rf  $(NAME)
	@echo  " 👾 Deleting PmergeMe."

re: fclean all
.PHONY: all clean fclean re