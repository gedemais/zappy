#ifndef ZAPPY_GETOPT_H
# define ZAPPY_GETOPT_H


# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>


typedef struct	zappy_client_opt_s
{
	char		*server_addr;
	char		*team_name;
	uint16_t	server_port;
}				zappy_client_opt_t;

int			zappy_client_getopt(int ac, char **av, zappy_client_opt_t *opt);


#endif
