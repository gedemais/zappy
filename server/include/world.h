#ifndef WORLD_H
# define WORLD_H

# define MAX_MAP_DIM_SIZE 200
# define LOOT_DENSITY 5
// Percentage of loot pieces destined to be food
# define FOOD_DENSITY 20

enum			e_cardinal_directions
{
	CDIR_N,
	CDIR_E,
	CDIR_S,
	CDIR_W,
	CDIR_NE,
	CDIR_SE,
	CDIR_SW,
	CDIR_NW,
	CDIR_MAX
};

// Minerals types
enum			e_minerals
{
	MIN_LINEMATE,
	MIN_DERAUMERE,
	MIN_SIBUR,
	MIN_MENDIANE,
	MIN_PHIRAS,
	MIN_THYSTAME,
	MIN_MAX
};

// Loot types
enum			e_loot
{
	LOOT_FOOD,
	LOOT_LINEMATE,
	LOOT_DERAUMERE,
	LOOT_SIBUR,
	LOOT_MENDIANE,
	LOOT_PHIRAS,
	LOOT_THYSTAME,
	LOOT_MAX,
	LOOT_PLAYER,
	LOOT_HATCHING_EGG
};


// Structure used as an abstraction of a tile of the game board
typedef struct	s_tile
{
	uint32_t	content[LOOT_HATCHING_EGG + 1];
	uint32_t	x; // Coordinates of the tile on the game board
	uint32_t	y;
}				t_tile;

typedef struct	s_team
{
	char		*name; // Team title
	t_dynarray	players; // Dynamic array of players
	uint16_t	connected; // Number of connections available in the team
	uint16_t	max_client; // Number of connections available in the team
}				t_team;

typedef struct	s_world
{
	t_dynarray	teams; // Dynamic array storing playing teams
	t_dynarray	eggs; // Dynamic array storing hatching eggs
	t_team		pending; // Dynamic array storing authentication-processing players
	t_tile		**map; // 2D map of tiles representing the game board
	uint32_t	food_items; // Used to threshold food generation trigger
}				t_world;

#endif
