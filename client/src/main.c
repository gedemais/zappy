#include "zap.h"
#include "zap_getopt.h"


int			main(int ac, char **av)
{
	zap_opt_t	opt = {0};
	int					r = 0;
	
	r = zap_getopt(ac, av, &opt);
	if (r == 0) {
		fprintf(stderr, "r=%d opt->ip={%s} opt->port={%d}\n", r, opt.server_addr, opt.server_port);
		r = zap(&opt);
	}
	return (r);
}
