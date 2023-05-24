#include "zappy_client_getopt.h"
#include "zappy_client.h"
#include <stdio.h>


int main(int ac, char **av)
{
	zappy_client_opt_t		opt = {0};
	int r = 0;
	
	r = zappy_client_getopt(ac, av, &opt);
	fprintf(stderr, "r=%d opt->ip={%s} opt->port={%d}\n", r, opt.server_addr, opt.server_port);
	if (r == 0)
	{
		r = zappy_client(&opt);
	}
	return (r);
}
