#ifndef MAIN_H
# define MAIN_H

# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <time.h>
# include <math.h>

# include "libft.h"

# include "error.h"
# include "world.h"
# include "player.h"

# define min(a,b) (((a)<(b))?(a):(b))
# define max(a,b) (((a)>(b))?(a):(b))

# define RESPONSE_SIZE pow(2, 20)


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

typedef struct	s_buffers
{
	char		*response;
	char		*cmd_param;
	t_dynarray	view;
}				t_buffers;

typedef	struct	s_env
{
	t_buffers	buffers;
	t_world		world; // See world.h
	t_settings	settings;
}				t_env;

typedef struct	s_cmd
{
	char		*response;
	uint16_t	cycles;
	uint8_t		(*cmd)(t_env *, t_player*);
}				t_cmd;

// Boilerplate
uint32_t	error(t_env *env, unsigned char code);
void		free_env(t_env *env);
uint8_t		parse_options(t_env *env, int argc, char **argv);

// World
uint8_t		init_world(t_env *env);
uint8_t		spawn_loot_pieces(t_env *env, uint32_t count);
void		print_map(t_env *env);

// Players
uint8_t		add_player(t_env *env, t_team *team);
void		update_food(t_player *p);
void		teams_log(t_env *env);

// Commands procedures
uint8_t		cmd_advance(t_env *env, t_player *p);
uint8_t		cmd_left(t_env *env, t_player *p);
uint8_t		cmd_right(t_env *env, t_player *p);
uint8_t		cmd_see(t_env *env, t_player *p);
uint8_t		cmd_inventory(t_env *env, t_player *p);
uint8_t		cmd_take(t_env *env, t_player *p);
uint8_t		cmd_put(t_env *env, t_player *p);

// See
uint8_t		add_tile_to_view(t_dynarray *view, t_dynarray *tile_content);
void		compute_view_ranges(t_env *env, t_view_ranges *ranges, t_player *p, uint8_t i);
void		send_see_response(t_env *env, t_dynarray *view);
void		free_view(t_dynarray *view);


// Core
uint8_t		tick(t_env *env);


static const int8_t	moves[DIR_MAX][2] ={{0, -1},
									{1, 0},
									{0, 1},
									{-1, 0}};

static const char	*loot_titles[LOOT_MAX] = {
	[LOOT_LINEMATE] = "linemate",
	[LOOT_DERAUMERE] = "deraumere",
	[LOOT_SIBUR] = "sibur",
	[LOOT_MENDIANE] = "mendiane",
	[LOOT_PHIRAS] = "phiras",
	[LOOT_THYSTAME] = "thystame",
	[LOOT_FOOD] = "nourriture"
};

static const char	*cmd_names[CMD_MAX] = {
					[CMD_ADVANCE] = "avance",
					[CMD_RIGHT] = "droit",
					[CMD_LEFT] = "gauche",
					[CMD_SEE] = "voir",
					[CMD_INVENTORY] = "inventaire",
					[CMD_TAKE] = "prend",
					[CMD_PUTDOWN] = "pose",
					[CMD_KICK] = "expulse",
					[CMD_BROADCAST] = "broadcast",
					[CMD_INCANTATION] = "incantation",
					[CMD_FORK] = "fork",
					[CMD_CONNECT_NBR] = "connect_nbr"
};

static const t_cmd	commands[CMD_MAX] = {
							[CMD_ADVANCE] = {.response = NULL, .cycles = 7, .cmd = &cmd_advance},
							[CMD_RIGHT] = {.response = NULL, .cycles = 7, .cmd = &cmd_left},
							[CMD_LEFT] = {.response = NULL, .cycles = 7, .cmd = &cmd_right},
							[CMD_SEE] = {.response = NULL, .cycles = 7, .cmd = &cmd_see},
							[CMD_INVENTORY] = {.response = NULL, .cycles = 1, .cmd = NULL},
							[CMD_TAKE] = {.response = NULL, .cycles = 7, .cmd = NULL},
							[CMD_PUTDOWN] = {.response = NULL, .cycles = 7, .cmd = NULL},
							[CMD_KICK] = {.response = NULL, .cycles = 7, .cmd = NULL},
							[CMD_BROADCAST] = {.response = NULL, .cycles = 7, .cmd = NULL},
							[CMD_INCANTATION] = {.response = NULL, .cycles = 300, .cmd = NULL},
							[CMD_FORK] = {.response = NULL, .cycles = 42, .cmd = NULL},
							[CMD_CONNECT_NBR] = {.response = NULL, .cycles = 0, .cmd = NULL}
};

#endif
