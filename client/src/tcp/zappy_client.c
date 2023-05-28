#include "zappy_client.h"


/* connection function, it handle all TCP stuff and Zappy connection protocol */
static int	zappy_client_connect(zappy_client_opt_t *opt, zappy_client_t *client)
{
	int r = 0;
	int len = 0;

	if (!opt || !client) {
		r = -1;
	}
	else
	{
    	if ((client->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
					client->player.pos_x = atoi((char*)&client->buf[i+1]);
				}
				if (client->buf[i] == ' ') {
					client->player.pos_y = atoi((char*)&client->buf[i+1]);
					break ;
				}
			}
		}
	}
	return (r);
}

// ============================================================================

int			zappy_client(zappy_client_opt_t *opt)
{
	int				r = 0;
	zappy_client_t	client = {0};

	if (!opt) {
		r = -1;
	}
	else
	{
		r = zappy_client_connect(opt, &client);
		if (r == 0) {
			fprintf(stderr, "\n===== client_connected =====\n\n");
			// copying team name in client structure (ptr to argv so no malloc)
			client.team.name = opt->team_name;
			client.player.alive = true;
			r = zappy_player(&client);
		}
		close(client.socket);
	}
	return (r);
}
