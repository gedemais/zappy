


SERVER = zappy_server
CLIENT = zappy_client
GRAPHIC = zappy_graphical_client

all: $(SERVER) $(CLIENT) $(GRAPHIC)

$(SERVER) :
		make -C server
		ln -sf server/server zappy_server

$(CLIENT) :
		make -C client
		ln -sf client/client zappy_client

$(GRAPHIC) :
		ln -sf graphical_client/client_v0.py zappy_graphical_client


clean :
	make -C server clean
	make -C client clean

fclean :
	make -C server fclean
	make -C client fclean
	rm -f $(SERVER) $(CLIENT) $(GRAPHIC)

re: fclean all
