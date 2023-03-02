#ifndef MAIN_H
# define MAIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdint.h>

# include "libft.h"

# include "error.h"
# include "world.h"

# define DEBUG true

// Server settings
typedef struct	s_settings
{
	uint16_t	map_width; // map dimensions in tiles
	uint16_t	map_height;
	uint16_t	port; // TCP port number
	uint16_t	nb_connections;
	uint16_t	max_connections;
	uint16_t	t;
	
}				t_settings;

typedef	struct	s_env
{
	t_world		world;
	t_settings	settings;
	
}				t_env;

uint32_t	error(t_env *env, unsigned char code);
void		free_env(t_env *env);

uint8_t		parse_options(t_env *env, int argc, char **argv);

#endif
