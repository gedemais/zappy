#include "main.h"

// Allocation of a 2D matrix representing each tile of the map
static uint8_t	allocate_map_tiles(t_env *env)
{
	// Allocating first dimension
	if (!(env->world.map = malloc(sizeof(t_tile*) * env->settings.map_height)))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; i < env->settings.map_height; i++)
	{
		// Allocating second dimension
		if (!(env->world.map[i] = malloc(sizeof(t_tile) * env->settings.map_width)))
			return (ERR_MALLOC_FAILED);
		memset(env->world.map[i], 0, sizeof(t_tile) * env->settings.map_width);
	}

	return (ERR_NONE);
}

// Sharing out resources on the map
static uint8_t	fill_resources(t_env *env)
{
	uint8_t		code;
	const int	nb_tiles = env->settings.map_width * env->settings.map_height;

	if ((code = spawn_loot_pieces(env, LOOT_DENSITY * nb_tiles)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}

void	print_map(t_env *env)
{
	uint8_t		*t;
	t_player	*p;
	bool		player;
	char		buffer[4096];
	uint16_t	index = 0;

	memset(buffer, 0, 4096);
	for (uint32_t y = 0; y < env->settings.map_height; y++)
	{
		for (uint32_t x = 0; x < env->settings.map_width; x++)
		{
			player = false;
			for (int i = 0; i < env->world.map[y][x].content.nb_cells; i++)
			{
				t = dyacc(&env->world.map[y][x].content, 0);
				if (*t == 255)
				{
					player = true;
					break ;
				}
			}
			strcat(&buffer[index], player ? "  " : "o ");
			index += 2;
		}
		strcat(&buffer[index], "\n");
		index++;
	}
	write(1, buffer, index);
	fflush(stdout);
}

// Initialization of world map
uint8_t	init_world(t_env *env)
{
	uint8_t	code;

	if ((code = allocate_map_tiles(env))
		|| (code = fill_resources(env)))
		return (code);

	return (ERR_NONE);
}
