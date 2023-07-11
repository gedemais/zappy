#ifndef PLAYER_H
# define PLAYER_H

# define LVL_MAX 8
# define MAX_QUEUED_CMD 10

// Commands identifiers enumeration
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

// Directions identifiers enumeration
enum			e_directions
{
	DIR_NORTH,
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
	DIR_MAX
};


// Directions identifiers enumeration
enum			e_broadcast_directions
{
	BDIR_NORTH,
	BDIR_NORTH_WEST,
	BDIR_WEST,
	BDIR_SOUTH_WEST,
	BDIR_SOUTH,
	BDIR_SOUTH_EAST,
	BDIR_EAST,
	BDIR_NORTH_EAST,
	BDIR_MAX
};

// Player's authentication steps identifiers
enum	e_auth_steps
{
	AS_NONE,
	AS_TEAM_NAME,
	AS_GRANTING,
	AS_MAX
};

// Used to localy compute view range
typedef struct	s_view_ranges
{
	int16_t		middle_x, middle_y, start_x, start_y, end_x, end_y;
}				t_view_ranges;

typedef struct s_direction // Useful for over / under flowing directions values
{
	unsigned char	d : 2;
}				t_direction;

typedef	struct	s_egg
{
	int32_t		id;
	uint16_t	team;
	uint16_t	hatch_time;
}				t_egg;

typedef struct	s_player
{
	t_dynarray	cmd_queue;
	int			*connection; // Connection fd
	uint16_t	inventory[LOOT_MAX]; // Inventory (quantity for each loot type)
	int16_t		tile_x, tile_y; // Position of the player in field (tile) unit
	uint16_t	team; // Index of the team the player is in
	uint8_t		satiety; // Current satiety of the player (126 when food eaten)
	uint8_t		level; // Current level of the player
	t_direction	direction; // Current cardinal orientation of the player
	bool		alive; // Is the player currently alive ?
	uint8_t		auth_step; // Which step of authentication has the player reached yet.
	int32_t		pid;
}				t_player;

// Minerals and players requirements for level up incantations
// Players number is in last position in the arrays to allow index-aligned access
static const uint8_t	lvl_up_requirements[LVL_MAX][LOOT_MAX + 1] = {
						{1, 0, 0, 0, 0, 0, 1},
						{1, 1, 1, 0, 0, 0, 2},
						{2, 0, 1, 0, 2, 0, 2},
						{1, 1, 2, 0, 1, 0, 4},
						{1, 2, 1, 3, 0, 0, 4},
						{1, 2, 3, 0, 1, 0, 6},
						{2, 2, 2, 2, 2, 1, 6}
};

#endif
