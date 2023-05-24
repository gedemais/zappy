#include "main.h"

static char		is_diagonal(t_player *sender, t_player *receiver)
{
	int		sender_slope, receiver_slope;
	char	dir;

	sender_slope = abs(sender->tile_x - sender->tile_y);
	receiver_slope = abs(receiver->tile_x - receiver->tile_y);

	if (sender_slope != receiver_slope)
		return (BDIR_MAX);

	if (sender->tile_x < receiver->tile_x && sender->tile_y < receiver->tile_y)
		dir = BDIR_NORTH_WEST;
	else if (sender->tile_x < receiver->tile_x && sender->tile_y > receiver->tile_y)
		dir = BDIR_SOUTH_WEST;
	else if (sender->tile_x > receiver->tile_x && sender->tile_y > receiver->tile_y)
		dir = BDIR_SOUTH_EAST;
	else
		dir = BDIR_NORTH_EAST;

	return (dir);
}

static char		get_direction(t_player *sender, t_player *receiver)
{
	char	dir = BDIR_MAX;
	int		rx, ry;
	float	m;

	m = (sender->tile_y - receiver->tile_y) / (sender->tile_x - receiver->tile_x);
	if (m < -1.0f || m > 1.0f)
	{
		if (receiver->tile_y < sender->tile_y) // NORTH
			dir = BDIR_NORTH;
		else if (sender->tile_y < receiver->tile_y) // SOUTH
			dir = BDIR_SOUTH;
	}
	else if (m >= -1.0f && m <= 1.0f)
	{
		if (receiver->tile_x < sender->tile_x) // EAST
			dir = BDIR_EAST;
		else if (sender->tile_x < receiver->tile_x) // WEST
			dir = BDIR_WEST;
	}
	if (dir == BDIR_MAX)
		dir = is_diagonal(sender, receiver); // Diagonales

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

	if ((dir = get_direction(sender, receiver)) == BDIR_MAX)
		exit(1);

	dir -= (unsigned char)receiver->direction.d;

	dir += (dir < 0) ? BDIR_MAX : 0;
	dir += (dir >= BDIR_MAX) ? -BDIR_MAX : 0;

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
			if (pl == p)
				continue;
			// Compute shortest path + direction and add it to response
			build_message(env, p, pl);
			response(env, pl);
		}
	}
	return (ERR_NONE);
}
