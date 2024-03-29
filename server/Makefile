include scripts/srcs.mk
include scripts/incs.mk

NAME = server

CC = gcc
FLAGS = -Wall -Werror -Wextra -Wno-unused-variable

# Os detection
OS = $(shell uname -s)

ifeq ($(OS), Darwin)
	FLAGS += -DMACOS
endif

ifeq ($(OS), Linux)
	FLAGS += -DLINUX -Wno-format-truncation
endif

# Flags variables
VALGRIND ?= 0
ifeq ($(VALGRIND), 1)
    FLAGS += -g
endif

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g3 -fsanitize=address
endif

OPTI ?= 0
ifeq ($(OPTI), 1)
    FLAGS += -Ofast -march=native -Wno-unused-result
endif

SRCS_PATH = src/
SRCS = $(addprefix $(SRCS_PATH), $(SRCS_NAME))

INCS_PATH = include/
INCS = $(addprefix $(INCS_PATH), $(INCS_NAME))

LIB_PATH = ../libft/
LIB = $(LIB_PATH)libft.a

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(FLAGS) -I $(INCS_PATH) -I $(LIB_PATH) -o $(NAME) $(OBJS) $(LIB) -lm

$(SRCS_PATH)%.o: $(SRCS_PATH)%.c $(INCS)
	@tput civis
	@printf " Compiling $<"
	@printf "                                       \\r"
	@tput cnorm
	$(CC) $(FLAGS) -I $(INCS_PATH) -I $(LIB_PATH) -o $@ -c $<

$(LIB): $(LIB_PATH)
	make -C $(LIB_PATH) -j4

clean:
	rm -rf $(OBJS)
	make -C $(LIB_PATH) clean

fclean: clean
	make -C $(LIB_PATH) fclean
	rm -rf $(NAME) $(NAME).dSYM
	rm -rf $(NAME).dSYM

re: fclean all
