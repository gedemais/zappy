#include "main.h"

uint8_t	cmd_advance(t_env *env, t_player *p)
{
	const int8_t	moves[DIR_MAX][2] ={{0, -1},
										{1, 0},
										{0, 1},
										{-1, 0}};

	for (uint32_t dir = 0; dir < DIR_MAX; dir++)
		if (p->direction == dir)
		{
			p->tile_x += moves[dir][0];
			p->tile_y += moves[dir][1];
			return (ERR_NONE);
		}

	return (ERR_INVALID_PLAYER_DIRECTION);
}

uint8_t	cmd_left(t_env *env, t_player *p)
{
	p->direction += 1;
	if (p->direction == DIR_MAX)
		p->direction = DIR_NORTH;
	return (ERR_NONE);
}

uint8_t	cmd_right(t_env *env, t_player *p)
{
	if (p->direction > DIR_NORTH)
		p->direction -= 1;
	else
		p->direction = DIR_WEST;
	return (ERR_NONE);
}

/*
uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}
/*
