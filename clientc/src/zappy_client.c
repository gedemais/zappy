#include "zappy_client_getopt.h"
#include "zappy_client.h"
#include <stdlib.h>

//static char see_cmd[]			= "voir\n";
static char advance_cmd[]		= "avance\n";
static char turn_right_cmd[]	= "droite\n";
//static char turn_left_cmd[]	= "gauche\n";
//static char inventory_cmd[]	= "inventaire\n";
//static char kick_cmd[]		= "expulse\n";
//static char incantation_cmd[]	= "incantation\n";
//static char fork_cmd[]		= "fork\n";
//static char connect_nbr_cmd[]	= "connect_nbr\n";

int	zappy_client_connect(zappy_client_opt_t *opt, zappy_client_t *client)
{
	int r = 0;
	int len = 0;

	if (!opt || !client)
	{
		r = -1;
	}
	else
	{
    	if ((client->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("socket");
			r = -1;
    	}
		if (r == 0)
		{
    		client->sockaddr.sin_family = AF_INET;
    		client->sockaddr.sin_port = htons(opt->server_port);
    		if (inet_pton(AF_INET, opt->server_addr, &client->sockaddr.sin_addr) <= 0) {
				perror("inet_pton");
				close(client->socket);
				r = -1;
    		}
		}
		if (r == 0)
		{
    		if (connect(client->socket,
						(struct sockaddr*)&client->sockaddr,
    		    		sizeof(struct sockaddr_in)) < 0) {
				perror("connect");
				close(client->socket);
				r = -1;
    		}
		}
		if (r == 0)
		{
			if (recv(client->socket, client->buf, CLIENT_BUFSIZE, 0) < 0) {
				perror("recv");
				close(client->socket);
				r = -1;
			}
		}
		if (r == 0)
		{
			if (strcmp((char*)client->buf, "BIENVENUE\n")) {
				fprintf(stderr, "%s: error BIENVENUE not recv\n", __func__);
				close(client->socket);
				r = -1;
			}
		}
		if (r == 0)
		{
			if (send(client->socket, opt->team_name, strlen(opt->team_name), 0) < 0) {
				perror("send");
				close(client->socket);
				r = -1;
			}
		}
		if (r == 0)
		{
			bzero(client->buf, CLIENT_BUFSIZE);
			if ((len = recv(client->socket, client->buf, CLIENT_BUFSIZE, 0)) < 0) {
				perror("recv");
				close(client->socket);
				r = -1;
			}
		}
		if (r == 0)
		{
			if (atoi((char*)client->buf) < 1) {
				fprintf(stderr, "%s: max client connection reached\n", __func__);
				r = -1;
			}
		}
		if (r == 0)
		{
			for (int i = 0 ; client->buf[i] ; i++) {
				if (client->buf[i] == '\n') {
					client->pos_x = atoi((char*)&client->buf[i+1]);
				}
				if (client->buf[i] == ' ') {
					client->pos_y = atoi((char*)&client->buf[i+1]);
					break ;
				}
			}
		}
	}
	return (r);
}

int	zappy_client_transceive(zappy_client_t *client, char *cmd, int len)
{
	int r = 0;

	bzero(client->buf, 4096);
	if (send(client->socket, cmd, len, 0) < 0) {
		perror("send");
		r = -1;
	}
	if (r == 0) {
		if (recv(client->socket, client->buf, CLIENT_BUFSIZE, 0) < 0) {
			perror("recv");
			r = -1;
		}
	}
	fprintf(stderr, "%s: client->buf = {%s}\n", __func__, client->buf);
	return (r);
}

int	zappy_client_fsm(zappy_client_t *client)
{
	int r = 0;
	bool run = true;

	while (run)
	{
		// r = zappy_client_see(client);
		//for (int i = 0 ; i < 16 ; i++) {
		//	fprintf(stderr, "vision[%d] = %02x\n", i, client->vision_map[i]);
		//}
		r = zappy_client_transceive(client, advance_cmd, strlen(advance_cmd));
		if (r == 0) {
			r = zappy_client_transceive(client, turn_right_cmd, strlen(turn_right_cmd));
		}
		if (r != 0) {
			run = false;
		}
	}
	return (r);
}

int zappy_client(zappy_client_opt_t *opt)
{
	int				r = 0;
	zappy_client_t	client = {0};

	if (!opt)
	{
		r = -1;
	}
	else
	{
		r = zappy_client_connect(opt, &client);

		fprintf(stderr, "client_connected r=%d pos_x=%d pos_y=%d\n", r, client.pos_x, client.pos_y);
		if (r == 0) {
			r = zappy_client_fsm(&client);
		}
		close(client.socket);
	}
	return (r);
}
