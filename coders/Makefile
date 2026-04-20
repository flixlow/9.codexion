NAME = codexion

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread -g3

SRC = main.c parsing.c error.c init.c monitor.c time.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.c codexion.h Makefile
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean bonus
