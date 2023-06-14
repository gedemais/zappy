#ifndef ZAPPY_GETOPT_H
# define ZAPPY_GETOPT_H


# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>


typedef struct	zap_opt_s
{
	char		*server_addr;
	char		*team_name;
	uint16_t	server_port;
	uint32_t 	max_x;
	uint32_t 	max_y;
}				zap_opt_t;

int			zap_getopt(int ac, char **av, zap_opt_t *opt);


#endif
