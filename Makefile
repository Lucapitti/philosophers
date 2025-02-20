NAME = philo
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-4
SRCS = philo/main.c philo/time.c philo/philo.c philo/threads
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
