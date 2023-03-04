#ifndef MAIN_H
# define MAIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <time.h>

# include "libft.h"

# include "error.h"
# include "world.h"
# include "player.h"

// Server settings
typedef struct	s_settings
{
	uint16_t	map_width; // Map dimensions in tiles
	uint16_t	map_height;
	uint16_t	port; // TCP port number
	uint16_t	nb_connections; // Current number of connected clients
	uint16_t	max_connections; // Maximum number of connections
	uint16_t	t; // Time unit (1 tick of the server lasts for 1/t second)
}				t_settings;

typedef	struct	s_env
{
	t_world		world; // See world.h
	t_settings	settings;
}				t_env;

// Boilerplate
uint32_t	error(t_env *env, unsigned char code);
void		free_env(t_env *env);
uint8_t		parse_options(t_env *env, int argc, char **argv);

// World
uint8_t		init_world(t_env *env);
uint8_t		spawn_loot_pieces(t_env *env, uint32_t count);

// Players
uint8_t		add_player(t_env *env, t_team *team);
void		update_food(t_player *p);
void		teams_log(t_env *env);

uint8_t		tick(t_env *env);

#endif
