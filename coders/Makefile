NAME = codexion

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread -g3

SRC = main.c parsing.c error.c

OBJ = $(SRC:.c=.o)

DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
		$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
		rm -f $(OBJ) $(DEP)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean bonus
