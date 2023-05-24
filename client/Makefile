


NAME = client
CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -g3

SRC = main.c \
	  	zappy_client.c \
	  	zappy_client_getopt.c \
		zappy_client_move.c \
		zappy_farmer.c

OBJ = $(addprefix obj/,$(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

obj/%.o:src/%.c $(HDR) Makefile
	@mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
