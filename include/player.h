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

typedef struct	s_cmd
{
	uint8_t		cmd_id;
	uint8_t		player_id;
	uint16_t	cycles;
}				t_cmd;

typedef struct s_direction // Useful for over / under flowing directions values
{
	unsigned char	d : 2;
}				t_direction;

typedef struct	s_player
{
	uint16_t	inventory[LOOT_MAX];
	int16_t		tile_x, tile_y; // Position of the player in field (tile) unit
	uint16_t	food; // Number of stored food (10 at start)
	uint8_t		satiety; // Current satiety of the player (126 when food eaten)
	uint8_t		level; // Current level of the player
	t_direction	direction; // Current cardinal orientation of the player
	bool		alive; // Is the player currently alive ?
}				t_player;


static const uint8_t	fov_width[LVL_MAX] = {3, 5, 7, 9, 11, 13, 15, 17};

#endif
