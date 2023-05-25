#include "zappy_client_getopt.h"
#include "zappy_client.h"
#include <stdlib.h>

// Zappy player, the happy-go-lucky farmer
int	zappy_player(zappy_client_t *client);

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

int		zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb)
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
		fprintf(stderr, "%s: send %s pos=%d orientation=%d\n", __func__, cmd, client->player.relative_pos, client->player.relative_orientation);
		if (send(client->socket, cmd, len, 0) < 0) {
			perror("send");
			r = -1;
		}
	}
	return (r);
}

int		zappy_client(zappy_client_opt_t *opt)
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
			fprintf(stderr, "client_connected pos_x=%d pos_y=%d\n", client.player.pos_x, client.player.pos_y);
			r = zappy_player(&client);
		}
		close(client.socket);
	}
	return (r);
}

int		zappy_handle_server_reponse(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "client receipt : response (%s) received for cmd={%s}\n", client->buf, client->cmds[client->cmd_idx].cmd);
	r = client->cmds[client->cmd_idx].cb(client);
	client->cmd_idx++;
 	client->cmd_idx %= ZAPPY_CLIENT_MAX_STACKED_CMD;
	client->cmd_stack_size--;
	return (r);
}

int		zappy_client_receipt(zappy_client_t *client)
{
	int r = 0;
	fd_set				read_fd_set;
	struct timeval		timeout = {.tv_sec = 0, .tv_usec = 100};

	FD_ZERO(&read_fd_set);
	FD_SET(client->socket, &read_fd_set);
	if ((r = select(1024 + 1, &read_fd_set, NULL, NULL, &timeout)) >= 0) // If select does not fail
	{
		if (r > 0)
		{
			// Data available
			if (recv(client->socket, client->buf, CLIENT_BUFSIZE, 0) < 0) {
				perror("recv");
				r = -1;
			}
			if (r != -1)
			{
				// ================================================================
				// permet de gerer les responses de l'API sur les call des callback
				// ================================================================

				if (!memcmp(client->buf, "mort", strlen("mort"))) {
					// mort
					// => kill le client
				}
				else if (!memcmp(client->buf, "deplacement", strlen("deplacement"))) {
					// deplacement <K> (K : direction dou provient le joueur)
					// => deplacer le joueur dans la direction inverse
				}
				// else if (!memcmp(client->buf, "voir", strlen("voir"))) {
				// 	// voir {case1,phiras sibur sibur,...}
				// }
				// else if (!memcmp(client->buf, "inventaire", strlen("inventaire"))) {
				// 	// inventaire {phiras n,sibur n,...} tick <K> lvl <V> (K : tick serveur à vivre, V : niveau actuel)
				// }
				// else if (!memcmp(client->buf, "prend", strlen("prend"))) {
				// 	// prend ok/ko
				// }
				// else if (!memcmp(client->buf, "pose", strlen("pose"))) {
				// 	// pose ok/ko
				// }
				// else if (!memcmp(client->buf, "expulse", strlen("expulse"))) {
				// 	// expulse ok/ko
				// }
				// else if (!memcmp(client->buf, "message", strlen("message"))) {
				// 	// message <K> texte <texte> (K : case d où provient le son)
				// }
				// else if (!memcmp(client->buf, "incantation", strlen("incantation"))) {
				// 	// incantation ok/ko lvl <K> (K : niveau actuel)
				// }
				// else if (!memcmp(client->buf, "fork", strlen("fork"))) {
				// 	// fork ok/ko
				// }
				// else if (!memcmp(client->buf, "connect_nbr", strlen("connect_nbr"))) {
				// 	// connect_nbr remain <K> used <V> (K : slot restant, V : slot utilisé)
				// 	// => remain permet de savoir si il faut hatch - used permet de connaitre l'id
				// }
				// else
				// {
				// 	// avance - droite - gauche
				// }
				zappy_handle_server_reponse(client);
			}

		}
		else if (r < 0) {
			r = -1;
		}
	}
	return (r);
}
