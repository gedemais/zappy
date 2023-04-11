#ifndef PLAYER_H
# define PLAYER_H

# define LVL_MAX 8

enum			e_commands
{
	CMD_ADVANCE,
	CMD_RIGHT,
	CMD_LEFT,
	CMD_SEE,
	CMD_INVENTORY,
	CMD_TAKE,
	CMD_PUTDOWN,
	CMD_KICK,
	CMD_BROADCAST,
	CMD_INCANTATION,
	CMD_FORK,
	CMD_CONNECT_NBR,
	CMD_MAX
};

enum			e_directions
{
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
	DIR_MAX
};

enum	e_auth_steps
{
	AS_NONE,
	AS_WELCOME,
	AS_TEAM_NAME,
	AS_NB_CLIENT,
	AS_X_Y,
	AS_MAX
};

typedef struct	s_view_ranges
{
	int16_t		middle_x, middle_y, start_x, start_y, end_x, end_y;
}				t_view_ranges;

typedef struct s_direction // Useful for over / under flowing directions values
{
	unsigned char	d : 2;
}				t_direction;

typedef struct	s_player
{
	int			connection;
	uint16_t	inventory[LOOT_MAX];
	int16_t		tile_x, tile_y; // Position of the player in field (tile) unit
	uint8_t		satiety; // Current satiety of the player (126 when food eaten)
	uint8_t		level; // Current level of the player
	t_direction	direction; // Current cardinal orientation of the player
	bool		alive; // Is the player currently alive ?
	uint8_t		auth_step; // Which step of authentication has the player reached yet.
}				t_player;

#endif
