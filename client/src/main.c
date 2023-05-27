#include "zappy_client.h"


int			main(int ac, char **av)
{
	zappy_client_opt_t	opt = {0};
	int					r = 0;
	
	r = zappy_client_getopt(ac, av, &opt);
	if (r == 0) {
		fprintf(stderr, "r=%d opt->ip={%s} opt->port={%d}\n", r, opt.server_addr, opt.server_port);
		r = zappy_client(&opt);
	}
	return (r);
}
