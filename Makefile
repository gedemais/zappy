SERVER = zappy_server
CLIENT = zappy_client
GRAPHIC = zappy_graphical_client

LIB_PATH = libft/
LIB = $(LIB_PATH)libft.a

all: $(LIB) $(SERVER) $(CLIENT) $(GRAPHIC)

$(SERVER):
	make -C server
	ln -sf server/server zappy_server

$(CLIENT):
	make -C client
	ln -sf client/client zappy_client

$(GRAPHIC):
	ln -sf graphical_client/client_v0.py zappy_graphical_client

$(LIB): $(LIB_PATH)
	make -C $(LIB_PATH) -j4

run:
	(sleep 3 && ./zappy_client -p 8080 -n foo) 2> $(TERM_CLIENT)  & ./zappy_server -p 8080 -x 30 -y 30 -n foo -c 3 -t 200 > $(TERM_SERVER)

clean:
	make -C server clean
	make -C client clean
	make -C $(LIB_PATH) clean

fclean:
	make -C server fclean
	make -C client fclean
	make -C $(LIB_PATH) fclean
	rm -f $(SERVER) $(CLIENT) $(GRAPHIC)

re: fclean all
