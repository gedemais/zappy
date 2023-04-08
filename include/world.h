#ifndef WORLD_H
# define WORLD_H

// Percentage of cases supposed to contain loot at every moment
# define LOOT_DENSITY 10
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

enum			e_loot
{
	LOOT_FOOD,
	LOOT_LINEMATE,
	LOOT_DERAUMERE,
	LOOT_SIBUR,
	LOOT_MENDIANE,
	LOOT_PHIRAS,
	LOOT_THYSTAME,
	LOOT_MAX
};

typedef struct	s_tile
{
	t_dynarray	content;
	uint32_t	x;
	uint32_t	y;
}				t_tile;

typedef struct	s_team
{
	char		*name;
	t_dynarray	players;
}				t_team;

typedef struct	s_world
{
	t_dynarray	teams;
	t_team		pending;
	t_tile		**map;
}				t_world;

#endif
