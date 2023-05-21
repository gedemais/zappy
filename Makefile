


SERVER = zappy_server
CLIENT = zappy_client
GRAPHIC = zappy_graphical_client

all: $(SERVER) $(CLIENT) $(GRAPHIC)

$(SERVER) :
		make -C server
		ln -sf server/server zappy_server

$(CLIENT) :
		ln -sf client/client_v0.py zappy_client

$(GRAPHIC) :
		ln -sf graphical_client/client_v0.py zappy_graphical_client


clean :
	make -C server clean

fclean :
	make -C server fclean
	rm -f $(SERVER) $(CLIENT) $(GRAPHIC)

re: fclean all
