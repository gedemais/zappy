#ifndef MAIN_H
# define MAIN_H

# include <assert.h>
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
# include "graphical.h"

# define min(a,b) (((a)<(b))?(a):(b))
# define max(a,b) (((a)>(b))?(a):(b))

# define PUTTIME() do {\
	struct timeval t;\
	gettimeofday(&t, NULL);\
	fprintf(stderr, "%ld.%06ld|", t.tv_sec, (long)t.tv_usec);\
	}\
	while (0);
	

# define DEV true

# define RESPONSE_SIZE 1048576
# define FLUSH_RESPONSE memset(env->buffers.response, 0, strlen(env->buffers.response));
# define FLUSH_GRESPONSE memset(env->buffers.gresponse, 0, strlen(env->buffers.gresponse));
# define REQUEST_BUFF_SIZE 8192

# define MAP_PRINT true
# define DATA_PRINT false

typedef struct s_env t_env;

typedef struct	s_cmd
{
	t_player		*p; // Player who sent the command
	int32_t			id;
	char			**tokens; // Word-tokens composing the command
	uint16_t		cycles; // Number of cycles remaining before to actually execute the instruction
	uint8_t			(*cmd_func)(t_env *, t_player*, bool); // Function pointer storing the address of the instruction related function
	int32_t			pid;
}					t_cmd;

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
	char		*response; // Buffer containing response text. Associated with FLUSH_RESPONSE macro.
	char		*gresponse; // Buffer containing graphical client response text. Associated with GFLUSH_RESPONSE macro.
	char		**cmd_params; // Params of the command received by the server (split by spaces)
	t_dynarray	view; // Dynamic array of dynamic arrays, representing the content of a view.
}				t_buffers;

typedef	struct	s_tcp
{
	int					server_fd; // Connection fd of the server
	int					socket; // Socket identifier
	uint16_t			server_port; // Port to listen on
	struct sockaddr_in	address;
}				t_tcp;

struct	s_env
{
	t_buffers	buffers; // Buffers containing various data useful to the server
	t_world		world; // See world.h
	t_tcp		tcp; // TCP logistic variables
	t_player	graphical;
	int			gx, gy, gindex, gpid, gnew_t;
	t_player	gplayer;
	char		*gstr;
	t_settings	settings;
	bool		start;
	bool		end;
	int32_t	consumed_food;
};

// Core
uint8_t		tick(t_env *env);
uint8_t		update_commands(t_env *env);
void		log_function(char *s);

// Memory free
void		free_env(t_env *env);
void		free_cmd(t_cmd *cmd);

// Error handling
uint32_t	error(t_env *env, unsigned char code);

// Server initialisation
uint8_t		init_server(t_env *env, int argc, char **argv);
uint8_t		parse_options(t_env *env, int argc, char **argv);

// Communication via TCP
uint8_t		init_tcp(t_env *env);
uint8_t		handle_connections(t_env *env);
uint8_t		receipt(t_env *env);
uint8_t		auth(t_env *env, t_player *p);
uint8_t		auth_send_welcome(t_env *env, t_player *p);
uint8_t		place_command_in_queue(t_env *env, t_player *player);
uint8_t		waiting_response(t_env *env, t_player *player);

// Graphical TCP connection
uint8_t		handle_graphical_connection(t_env *env, t_player *p);
uint8_t		graphical_request(t_env *env);

uint8_t		send_graphical_data(t_env *env);

// Graphical client commands
uint8_t		gcmd_map_size(t_env *env);
uint8_t		gcmd_map_content(t_env *env);
uint8_t		gcmd_server_time_unit(t_env *env);
uint8_t		gcmd_block_content(t_env *env);
uint8_t		gcmd_teams_names(t_env *env);
uint8_t		gcmd_player_new(t_env *env);
uint8_t		gcmd_player_position(t_env *env);
uint8_t		gcmd_player_level(t_env *env);
uint8_t		gcmd_player_inventory(t_env *env);
uint8_t		gcmd_set_new_t(t_env *env);

// Graphical events notifications
uint8_t		gevent_player_new(t_env *env);
uint8_t		gevent_player_expulse(t_env *env);
uint8_t		gevent_player_broadcast(t_env *env);
uint8_t		gevent_player_lays_egg(t_env *env);
uint8_t		gevent_player_put(t_env *env);
uint8_t		gevent_player_take(t_env *env);
uint8_t		gevent_player_died(t_env *env);
uint8_t		gevent_player_layed_egg(t_env *env);
uint8_t		gevent_egg_hatched(t_env *env);
uint8_t		gevent_player_connected_for_egg(t_env *env);
uint8_t		gevent_egg_rotted(t_env *env);
uint8_t		gevent_game_ended(t_env *env);
uint8_t		gevent_broadcast(t_env *env);
uint8_t		gevent_incantation_launch(t_env *env);
uint8_t		gevent_incantation_ended(t_env *env);

// Graphical tools
uint8_t		cat_spaced_number(t_env *env, int n, bool newline);
uint8_t		update_inventories(t_env *env);


// World
uint8_t		init_world(t_env *env);
uint8_t		spawn_loot_pieces(t_env *env, uint32_t count);
void		print_map(t_env *env);

// Players
uint8_t		add_player(t_env *env, t_team *team, int *connection, t_player *pending);
uint8_t		kill_player(t_env *env, t_player *p, bool disconnected);
uint8_t		remove_player(t_env *env, int connection_fd);
uint8_t		update_players(t_env *env);
void		teams_log(t_env *env, bool log);
uint8_t		send_response(t_env *env, t_player *p, char *s);
uint8_t		response(t_env *env, t_player *p);
uint8_t		gresponse(t_env *env);

/* * * * * Commands procedures * * * * */
uint8_t		check_requirements(t_env *env, char **tokens, t_player *player, int cmd, bool *ret);

uint8_t		cmd_incantation(t_env *env, t_player *p, bool send_response);

// Moving
uint8_t		cmd_advance(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_left(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_right(t_env *env, t_player *p, bool send_response);

// See command

uint8_t	view_x(t_env *env, t_player *p);
uint8_t	view_y(t_env *env, t_player *p);

uint8_t		cmd_see(t_env *env, t_player *p, bool send_response);
uint8_t		add_tile_to_view(t_dynarray *view, t_dynarray *tile_content);
void		send_see_response(t_env *env, t_dynarray *view, t_player *p);
void		free_view(t_dynarray *view);

uint8_t		cmd_inventory(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_take(t_env *env, t_player *p, bool send_response);
uint8_t		cmd_put(t_env *env, t_player *p, bool send_response);

// Kick command
uint8_t		cmd_kick(t_env *env, t_player *p, bool send_response);
uint16_t	count_players_on_tile(t_env *env, int16_t tile_x, int16_t tile_y);
void		kick_players(t_env *env, int16_t tile_x, int16_t tile_y, uint16_t to_kick);

// Broadcast command
uint8_t		cmd_broadcast(t_env *env, t_player *p, bool send_response);
uint8_t		deliver_messages(t_env *env, t_player *p);

// Connect number command
uint8_t		cmd_connect_nbr(t_env *env, t_player *p, bool send_response);

// Fork command
uint8_t		cmd_fork(t_env *env, t_player *p, bool send_response);
uint8_t		update_eggs(t_env *env);
uint8_t		hatch_egg(t_env *env, t_player *p);
uint8_t		check_connected_egg(t_env *env, t_player *p);

// Tools
t_player	*get_player_from_tile(t_env *env, int x, int y);
t_player	*get_pending_client(t_env *env, int client_fd);
t_player	*get_team_client(t_env *env, int client_fd);
uint8_t		remove_player_from_tile(t_env *env, int x, int y);
uint8_t		send_ko(t_env *env, t_player *p);
uint8_t		send_ok(t_env *env, t_player *p);
bool		is_only_number(char *s);

/* * * * * * * * * * * * * * * * * */


// Directions matched with indices corresponding to e_cardinal_directions members
static const uint8_t	directions[CDIR_MAX] = {1, 8, 7, 6, 5, 4, 3, 2};

// 2D vectors array representing a position offset in four different directions
static const int8_t		moves[DIR_MAX][2] ={{0, -1},
									{1, 0},
									{0, 1},
									{-1, 0}};

// Strings array storing names of loot elements
static const char	*loot_titles[LOOT_HATCHING_EGG] = {
	[LOOT_LINEMATE] = "linemate",
	[LOOT_DERAUMERE] = "deraumere",
	[LOOT_SIBUR] = "sibur",
	[LOOT_MENDIANE] = "mendiane",
	[LOOT_PHIRAS] = "phiras",
	[LOOT_THYSTAME] = "thystame",
	[LOOT_FOOD] = "nourriture",
	[LOOT_PLAYER] = "player"
};

// Strings array storing names of commands
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

// Structures array storing the different features of each executable command
static const t_cmd	commands[CMD_MAX] = {
							[CMD_ADVANCE] = {.cycles = 7, .cmd_func = &cmd_advance},
							[CMD_RIGHT] = {.cycles = 7, .cmd_func = &cmd_left},
							[CMD_LEFT] = {.cycles = 7, .cmd_func = &cmd_right},
							[CMD_SEE] = {.cycles = 7, .cmd_func = &cmd_see},
							[CMD_INVENTORY] = {.cycles = 1, .cmd_func = &cmd_inventory},
							[CMD_TAKE] = {.cycles = 7, .cmd_func = &cmd_take},
							[CMD_PUTDOWN] = {.cycles = 7, .cmd_func = &cmd_put},
							[CMD_KICK] = {.cycles = 7, .cmd_func = &cmd_kick},
							[CMD_BROADCAST] = {.cycles = 7, .cmd_func = &cmd_broadcast},
							[CMD_INCANTATION] = {.cycles = 300, .cmd_func = cmd_incantation},
							[CMD_FORK] = {.cycles = 42, .cmd_func = cmd_fork},
							[CMD_CONNECT_NBR] = {.cycles = 0, .cmd_func = cmd_connect_nbr}
};

// Graphical functions pointers array
static uint8_t	(*gcmds[GCMD_MAX])(t_env *) = {

	[GCMD_MAP_SIZE] = gcmd_map_size,
	[GCMD_BLOCK_CONTENT] = gcmd_block_content,
	[GCMD_MAP_CONTENT] = gcmd_map_content,
	[GCMD_TEAMS_NAME] = gcmd_teams_names,
	[GCMD_PLAYER_POSITION] = gcmd_player_position,
	[GCMD_PLAYER_LEVEL] = gcmd_player_level,
	[GCMD_PLAYER_INVENTORY] = gcmd_player_inventory,
	[GCMD_SGT] = gcmd_server_time_unit,
	[GCMD_SST] = gcmd_set_new_t
};

#endif
