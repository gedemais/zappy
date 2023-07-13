SERVER = zappy_server
CLIENT = zappy_client
GRAPHIC = zappy_graphical_client

LIB_PATH = libft/
LIB = $(LIB_PATH)libft.a

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

all: $(LIB) $(SERVER) $(GRAPHIC)

$(SERVER):
	make -C server OPTI=$(OPTI) DEBUG=$(DEBUG)
	ln -sf server/server zappy_server

$(GRAPHIC):
	ln -sf graphical_client/client_v0.py zappy_graphical_client

$(LIB): $(LIB_PATH)
	make -C $(LIB_PATH) -j4 OPTI=$(OPTI) DEBUG=$(DEBUG)

clean:
	make -C server clean
	make -C $(LIB_PATH) clean

fclean:
	make -C server fclean
	make -C $(LIB_PATH) fclean
	rm -f $(SERVER) $(GRAPHIC)

re: fclean all
