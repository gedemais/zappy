#include "zappy_client_getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int client_help(zappy_client_opt_t *opt, char *arg);
static int client_ip(zappy_client_opt_t *opt, char *arg);
static int client_port(zappy_client_opt_t *opt, char *arg);
static int client_team(zappy_client_opt_t *opt, char *arg);

static char 	*flags_list[] = {
	"--help",
	"-h",
	"-p",
	"-n",
	NULL
};

static char	*flags_help[] = {
	"--help: display this help",
	"-h <ip> : ip of the server",
	"-p <port> : port of the server",
	"-n <team name> : name of the client team",
	NULL
};

static int	(*flags_handler[])(zappy_client_opt_t *, char *) = {
	client_help,
	client_ip,
	client_port,
	client_team,
	NULL
};

static int client_help(zappy_client_opt_t *opt, char *arg)
{
	(void)opt;
	(void)arg;
	printf("Usage: zappy_client -p [port] -n [team] [-h [ip]]\n");
	for (int i = 0 ; flags_help[i]; i++) {
		printf("%s\n", flags_help[i]);
	}
	return (-1);
}
static int client_ip(zappy_client_opt_t *opt, char *arg)
{
	opt->server_addr = arg;
	return (2);
}
static int client_port(zappy_client_opt_t *opt, char *arg)
{
	opt->server_port = (uint16_t)atoi(arg);
	return (2);
}
static int client_team(zappy_client_opt_t *opt, char *arg)
{
	opt->team_name = arg;
	return (2);
}

int	zappy_client_getopt(int ac, char **av, zappy_client_opt_t *opt)
{
	int		i = 1;
	int 	r = 0;
	bool	b = true;

	while (i < ac && r == 0)
	{
		b = false;
		for (int j = 0 ; r == 0 && flags_list[j] ; j++)
		{
			if (!strcmp(flags_list[j], av[i]))
			{
				r = (flags_handler[j])(opt,
						(i + 1 != ac ? av[i + 1] : NULL));
				if (r > 0) {
					b = true;
					i += r;
					r = 0;
				}
				break ;
			}
		}
		if (b == false)
		{
			fprintf(stderr, "%s: unknow opt [%s]\n", __func__, av[i]);
			r = -1;
		}
	}
	if (!opt->server_addr)
	{
		opt->server_addr = "127.0.0.1";
	}
	if (!opt->server_addr || !opt->server_port || !opt->team_name)
	{
		client_help(opt, NULL);
		r = -1;
	}
	return (r);
}
