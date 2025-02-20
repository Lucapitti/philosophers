NAME = philo
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-4
SRCS =srcs/main.c srcs/philo.c srcs/threads.c srcs/utils.c
OBJ	= $(patsubst %.c, %.o, $(SRCS))
CC = gcc
RM = rm -f
INC = -I./


$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -lpthread -o $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

all: $(NAME)

clean:
	@$(RM) ${OBJ}

fclean: clean
	@$(RM) $(NAME)

re: fclean all

Std: $(SRCS)
	@for file in $^; do vim -c ":Std" -c ":wq" $$file; done


.PHONY: all clean fclean re
