#ifndef MAIN_H
# define MAIN_H

# include <ctype.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <stdint.h>
# include <time.h>
# include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

# include "libft.h"

# include "error.h"
# include "world.h"
# include "player.h"

# define min(a,b) (((a)<(b))?(a):(b))
# define max(a,b) (((a)>(b))?(a):(b))

# define RESPONSE_SIZE pow(2, 20)
# define FLUSH_RESPONSE memset(env->buffers.response, 0, strlen(env->buffers.response));
# define REQUEST_BUFF_SIZE 128

# define MAP_PRINT true
# define DATA_PRINT false

typedef struct s_env t_env;

typedef struct	s_cmd
{
	char		**tokens;
	char		*response;
	uint16_t	cycles;
	uint8_t		(*cmd_func)(t_env *, t_player*, bool);
}				t_cmd;

// Server settings
typedef struct	s_settings
{
	uint16_t	map_width; // Map dimensions in tiles
	uint16_t	map_height;
	uint16_t	nb_connections; // Current number of connected clients
	uint16_t	max_connections; // Maximum number of connections
	uint16_t	t; // Time unit (1 tick of the server lasts for 1/t second)
	useconds_t	tick_length;
}				t_settings;

typedef struct	s_buffers
{
	int			*connections; // Clients sockets file descriptors
	char		*request; // Buffer containing client-sent requests.
	char		*response; // Buffer containing response text. Associated with FLUSH_BUFFER macro.
	char		**cmd_params; // Params of the command received by the server (split by spaces)
	t_dynarray	cmd_queue;
	t_dynarray	view; // Dynamic array of dynamic arrays, representing the content of a view.
}				t_buffers;

typedef	struct	s_tcp
{
	int					server_fd;
	int					socket;
	uint16_t			server_port;
	struct sockaddr_in	address;
}				t_tcp;

struct	s_env
{
	t_buffers	buffers;
	t_world		world; // See world.h
	t_tcp		tcp;
	t_settings	settings;
};

// Boilerplate
void		free_env(t_env *env);
void		free_cmd(t_cmd *cmd);

uint32_t	error(t_env *env, unsigned char code);

uint8_t		parse_options(t_env *env, int argc, char **argv);
uint8_t		init_server(t_env *env, int argc, char **argv);


// Communication via TCP
uint8_t		init_tcp(t_env *env);
uint8_t		handle_connections(t_env *env);
uint8_t		receipt(t_env *env);
uint8_t		auth(t_env *env);

// World
uint8_t		init_world(t_env *env);
uint8_t		spawn_loot_pieces(t_env *env, uint32_t count);
void		print_map(t_env *env);

// Players
uint8_t		add_player(t_env *env, t_team *team, int connection);
void		update_food(t_player *p);
void		teams_log(t_env *env);
uint8_t		response(t_env *env, t_player *p);

// Commands procedures
uint8_t		cmd_advance(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_left(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_right(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_see(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_inventory(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_take(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_put(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_kick(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_broadcast(t_env *env, t_player *p, bool send_response);

// See
uint8_t		add_tile_to_view(t_dynarray *view, t_dynarray *tile_content);
void		compute_view_ranges(t_env *env, t_view_ranges *ranges, t_player *p, uint8_t i);
void		send_see_response(t_env *env, t_dynarray *view, t_player *p);
void		free_view(t_dynarray *view);

// Kick
uint16_t	count_players_on_tile(t_env *env, int16_t tile_x, int16_t tile_y);
void		kick_players(t_env *env, int16_t tile_x, int16_t tile_y, uint16_t to_kick);

// Broadcast
uint8_t		build_message_from_params(t_env *env);
uint8_t		deliver_message(t_env *env, t_player *p);

// Core
uint8_t		tick(t_env *env);
uint8_t		update_commands(t_env *env);


static const uint8_t	directions[CDIR_MAX] = {3, 1, 7, 5, 2, 8, 6, 4};

static const int8_t		moves[DIR_MAX][2] ={{0, -1},
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
					[CMD_RIGHT] = "droite",
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
							[CMD_ADVANCE] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_advance},
							[CMD_RIGHT] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_left},
							[CMD_LEFT] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_right},
							[CMD_SEE] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_see},
							[CMD_INVENTORY] = {.response = NULL, .cycles = 1, .cmd_func = &cmd_inventory},
							[CMD_TAKE] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_take},
							[CMD_PUTDOWN] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_put},
							[CMD_KICK] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_kick},
							[CMD_BROADCAST] = {.response = NULL, .cycles = 7, .cmd_func = &cmd_broadcast},
							[CMD_INCANTATION] = {.response = NULL, .cycles = 300, .cmd_func = NULL},
							[CMD_FORK] = {.response = NULL, .cycles = 42, .cmd_func = NULL},
							[CMD_CONNECT_NBR] = {.response = NULL, .cycles = 0, .cmd_func = NULL}
};

#endif
