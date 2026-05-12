NAME = codexion

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread -g3

SRC = coders/main.c \
		coders/parsing.c \
		coders/ending.c \
		coders/init.c \
		coders/monitor.c \
		coders/utils.c \
		coders/action.c \
		coders/routine.c \
		coders/scheduler.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.c coders/codexion.h Makefile
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all re fclean clean bonus
