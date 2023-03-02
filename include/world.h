#ifndef WORLD_H
# define WORLD_H

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
	t_tile		**map;
}				t_world;

#endif
