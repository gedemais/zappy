#include "main.h"

static char		diagonals(t_player *sender, t_player *receiver)
{
	char	dir = BDIR_MAX;

	if (receiver->tile_x > sender->tile_x && receiver->tile_y > sender->tile_y)
		dir = BDIR_SOUTH_EAST;
	else if (receiver->tile_x > sender->tile_x && receiver->tile_y < sender->tile_y)
			dir = BDIR_NORTH_EAST;
	else if (receiver->tile_x < sender->tile_x && receiver->tile_y > sender->tile_y)
		dir = BDIR_SOUTH_WEST;
	else if (receiver->tile_x < sender->tile_x && receiver->tile_y < sender->tile_y)
		dir = BDIR_NORTH_WEST;
	return (dir);
}

static char		get_direction(t_player *sender, t_player *receiver)
{
	char	dir = BDIR_MAX;
	float	rx = sender->tile_x - receiver->tile_x;
	float	ry = sender->tile_y - receiver->tile_y;
	float	m;

	if (fabs(rx) == fabs(ry))
	{
		fprintf(stderr, "[DIAGONAL BROADCAST DETECTED] rx = %f | ry = %f\n", rx, ry);
		fflush(stderr);
		return (diagonals(sender, receiver));
	}

//	printf("sender : %d %d | receiver : %d %d\n", sender->tile_x, sender->tile_y, receiver->tile_x, receiver->tile_y);

	m = ry / rx;
//	printf("%f / %f = %f\n", ry, rx, m);
	if (m < -1.0f || m > 1.0f)
	{
		if (receiver->tile_y > sender->tile_y) // NORTH
			dir = BDIR_NORTH;
		else if (sender->tile_y > receiver->tile_y) // SOUTH
			dir = BDIR_SOUTH;
	}
	else if (m >= -1.0f && m <= 1.0f)
	{
		if (receiver->tile_x < sender->tile_x) // EAST
			dir = BDIR_EAST;
		else if (sender->tile_x < receiver->tile_x) // WEST
			dir = BDIR_WEST;
	}

	//printf("dir : %d\n", dir);
	return (dir);
}

static void		concat_reception_direction(t_env *env, t_player *sender, t_player *receiver)
{
	const char	*names[BDIR_MAX] = {
							[BDIR_NORTH] = "1",
							[BDIR_NORTH_WEST] = "2",
							[BDIR_WEST] = "3",
							[BDIR_SOUTH_WEST] = "4",
							[BDIR_SOUTH] = "5",
							[BDIR_SOUTH_EAST] = "6",
							[BDIR_EAST] = "7",
							[BDIR_NORTH_EAST] = "8",
						};
	char		dir;

	if (receiver->tile_x == sender->tile_x
		&& receiver->tile_y == sender->tile_y)
	{
		strcat(env->buffers.response, "0");
		strcat(env->buffers.response, ",");
		return ;
	}

	else if ((dir = get_direction(sender, receiver)) == BDIR_MAX)
	{
		printf("CANT FIND_DIRECTION\n");
		assert(false);
	}


	if (receiver->direction.d == DIR_EAST)
		dir -= BDIR_EAST;
	else if (receiver->direction.d == DIR_SOUTH)
		dir -= BDIR_SOUTH;
	else if (receiver->direction.d == DIR_WEST)
		dir -= BDIR_WEST;

	//printf("dir : %d\n", dir);
//	if (abs(sender->tile_x - receiver->tile_x) < env->settings.map_width / 2.0f
//		&& abs(sender->tile_y - receiver->tile_y) < env->settings.map_height / 2.0f)
//		dir -= 4;

	if (dir < 0)
		dir += BDIR_MAX;

	if (dir >= BDIR_MAX)
		dir -= BDIR_MAX;

	//printf("dir : %d\n", dir);

	//fflush(stdout);
	//sleep(120);

	strcat(env->buffers.response, names[(int)dir]);
	strcat(env->buffers.response, ",");
}

static uint8_t	build_message(t_env *env, t_player *sender, t_player *receiver)
{
	uint16_t	i = 0;

	FLUSH_RESPONSE
	strcat(env->buffers.response, "message ");
	concat_reception_direction(env, sender, receiver);
	while (env->buffers.cmd_params[i])
	{
		strcat(env->buffers.response, env->buffers.cmd_params[i]);
		strcat(env->buffers.response, env->buffers.cmd_params[i + 1] ? " " : "\n");
		i++;
	}
	return (ERR_NONE);
}

uint8_t	deliver_messages(t_env *env, t_player *p)
{
	t_team		*te;
	t_player	*pl;

	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		te = dyacc(&env->world.teams, team);
		for (int player = 0; player < te->players.nb_cells; player++)
		{
			pl = dyacc(&te->players, player);
			if (pl == p || pl == NULL)
				continue;

			// Compute shortest path + direction and add it to response
			build_message(env, p, pl);
			response(env, pl);
		}
	}
	return (ERR_NONE);
}
