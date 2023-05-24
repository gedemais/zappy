#include "zappy_client_getopt.h"
#include "zappy_client.h"
#include <stdlib.h>

// Zappy farmer, the happy-go-lucky farmer
int zappy_farmer(zappy_client_t *client);

/* connection function, it handle all TCP stuff and Zappy connection protocol */
static int	zappy_client_connect(zappy_client_opt_t *opt, zappy_client_t *client)
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

int	zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb)
{
	int r = 0;

	if (client->cmd_stack_size >= ZAPPY_CLIENT_MAX_STACKED_CMD)
	{
		r = ZAPPY_WAIT;
	}
	if (r == 0)
	{
		client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].cmd = cmd;
		client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].cb = cb;
		client->cmd_stack_size++;
		bzero(client->buf, 4096); // TODO LMA
		if (send(client->socket, cmd, len, 0) < 0) {
			perror("send");
			r = -1;
		}
	}
	return (r);
}

int	zappy_client_parse_see(zappy_client_t *client)
{
	int r = 0;
	int i = 0;
	int c = 0;

	bzero(client->vision_map, VISION_MAP_MAX * CASE_ELEMENTS);
	client->relative_pos = 0;
	client->relative_orientation = 0;
	if (client->buf[i++] != '{') {
		r = -1;
	}
	while (r == 0 && i < (int)strlen((char*)client->buf))
	{
		if (client->buf[i] == '}') {
			break ;
		}
		else if (client->buf[i] == ',') {
			c++;
		}
		else if (client->buf[i] != ' ')
		{
			bool b = false;
			for (int j = 0 ; case_ressources[j] ; j++) {
				if (!memcmp(case_ressources[j],
							(char*)&client->buf[i], strlen(case_ressources[j]))) {
					b = true;
					client->vision_map[c * CASE_ELEMENTS + j]++;
					i += (int)strlen(case_ressources[j]);
				}
			}
			if (b == false)
			{
				r = -1;
			}
		}
		i++;
	}
	return (r);
}

int	zappy_client_see_cb(zappy_client_t *client)
{
	int r = 0;

	r = zappy_client_parse_see(client);
	if (r == 0)
	{
		client->task = ZAPPY_FARMER_LOOT;
		zappy_debug_print_vision_map(client);
	}
	return (r);
}

void zappy_debug_print_vision_map(zappy_client_t *client)
{
	for (int i = 0 ; i < VISION_MAP_MAX ; i++) {
		fprintf(stderr, "CASE[%3d] : L:%d D:%d S:%d M:%d P:%d T:%d F:%d P:%d\n", i,
				client->vision_map[i * CASE_ELEMENTS],
				client->vision_map[i * CASE_ELEMENTS + 1],
				client->vision_map[i * CASE_ELEMENTS + 2],
				client->vision_map[i * CASE_ELEMENTS + 3],
				client->vision_map[i * CASE_ELEMENTS + 4],
				client->vision_map[i * CASE_ELEMENTS + 5],
				client->vision_map[i * CASE_ELEMENTS + 6],
				client->vision_map[i * CASE_ELEMENTS + 7]);
	}
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

		if (r == 0) {
			fprintf(stderr, "client_connected pos_x=%d pos_y=%d\n", client.pos_x, client.pos_y);
			r = zappy_farmer(&client);
		}
		close(client.socket);
	}
	return (r);
}

int zappy_client_receipt(zappy_client_t *client)
{
	int r = 0;
	fd_set				read_fd_set;
	struct timeval		timeout = {.tv_sec = 0, .tv_usec = 100};

	FD_ZERO(&read_fd_set);
	FD_SET(client->socket, &read_fd_set);
	if ((r = select(1024 + 1, &read_fd_set,
					NULL, NULL, &timeout)) >= 0) // If select does not fail
	{
		if (r > 0)
		{
			// Data available
			if (recv(client->socket, client->buf,
						CLIENT_BUFSIZE, 0) < 0) {
				perror("recv");
				r = -1;
			}
			if (r != -1)
			{
				if (!memcmp(client->buf, "mort", strlen("mort"))) {
					fprintf(stderr, "death recv\n"); // TODO
				}
				else if (!memcmp(client->buf, "message", strlen("message"))) {
					fprintf(stderr, "broadcasted msg recv\n"); // TODO
				}
				else if (!memcmp(client->buf, "deplacement", strlen("deplacement"))) {
					fprintf(stderr, "kick recv\n"); // TODO
				}
				else
				{
					fprintf(stderr, "%s: response (%s) received for cmd={%s}", __func__, client->buf, client->cmds[client->cmd_idx].cmd);
					r = client->cmds[client->cmd_idx].cb(client);
					client->cmd_idx++;
 					client->cmd_idx %= ZAPPY_CLIENT_MAX_STACKED_CMD;
					client->cmd_stack_size--;
				}
			}

		}
		else if (r < 0)
		{
			r = -1;
		}
	}
	return (r);
}