#include "zap_getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int zap_getopt_help(zap_opt_t *opt, char *arg);
static int zap_getopt_ip(zap_opt_t *opt, char *arg);
static int zap_getopt_port(zap_opt_t *opt, char *arg);
static int zap_getopt_team(zap_opt_t *opt, char *arg);
static int zap_getopt_instance(zap_opt_t *opt, char *arg);

static char 	*flags_list[] = {
	"--help",
	"-h",
	"-p",
	"-n",
	"-i",
	NULL
};

static char	*flags_help[] = {
	"--help: display this help",
	"-h <ip> : ip of the server",
	"-p <port> : port of the server",
	"-n <team name> : name of the client team",
	"-i <instance nb> : nb of instance (default=1 max=6)",
	NULL
};

static int	(*flags_handler[])(zap_opt_t *, char *) = {
	zap_getopt_help,
	zap_getopt_ip,
	zap_getopt_port,
	zap_getopt_team,
	zap_getopt_instance,
	NULL
};

static int zap_getopt_help(zap_opt_t *opt, char *arg)
{
	(void)opt;
	(void)arg;
	printf("Usage: zap -p [port] -n [team] [-h [ip]]\n");
	for (int i = 0 ; flags_help[i]; i++) {
		printf("%s\n", flags_help[i]);
	}
	return (-1);
}
static int zap_getopt_ip(zap_opt_t *opt, char *arg)
{
	opt->server_addr = arg;
	return (2);
}
static int zap_getopt_port(zap_opt_t *opt, char *arg)
{
	opt->server_port = (uint16_t)atoi(arg);
	return (2);
}
static int zap_getopt_team(zap_opt_t *opt, char *arg)
{
	opt->team_name = arg;
	return (2);
}
static int zap_getopt_instance(zap_opt_t *opt, char *arg)
{
	opt->instance = atoi(arg);
	if (opt->instance <= 0)
		opt->instance = 1;
	if (opt->instance > 6)
		opt->instance = 6;
	return (2);
}

int	zap_getopt(int ac, char **av, zap_opt_t *opt)
{
	int		i = 1;
	int 	r = 0;
	bool	b = true;

	opt->instance = 3;
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
		zap_getopt_help(opt, NULL);
		r = -1;
	}
	return (r);
}
