NAME = codexion

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread

SRC = main.c 

OBJ = $(SRC:.c=.o)

DEP = $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
		$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
		rm -f $(OBJ) $(DEP)
		rm -f $(OBJ_BONUS) $(DEP_BONUS)

fclean: clean
		rm -f $(NAME)
		rm -f $(NAME_BONUS)

re: fclean all

.PHONY: all re fclean clean bonus
