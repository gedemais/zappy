#include "main.h"

void	send_see_response(t_env *env, t_dynarray *view, t_player *p)
{
	t_tile	*tile;

	FLUSH_RESPONSE
	strcat(env->buffers.response, "{");
	for (int i = 0; i < view->nb_cells; i++)
	{
		tile = dyacc(view, i);
		strcat(env->buffers.response, i > 0 ? ", " : "");
		for (int j = 0; j <= LOOT_PLAYER; j++)
		{
			for (uint8_t k = 0; k < tile->content[j]; k++)
			{
				strcat(env->buffers.response, loot_titles[j]);

				strcat(env->buffers.response, " ");
			}
		}
	}
	strcat(env->buffers.response, "}\n");
	response(env, p);
}

void	compute_view_ranges(t_env *env, t_view_ranges *ranges, t_player *p, uint8_t i)
{
	i += 1;
	ranges->middle_x = p->tile_x + moves[p->direction.d][0] * i;
	ranges->middle_y = p->tile_y + moves[p->direction.d][1] * i;

	clamp(&ranges->middle_x, 0, env->settings.map_width);
	clamp(&ranges->middle_y, 0, env->settings.map_height);

	p->direction.d -= 1;
	ranges->start_x = ranges->middle_x + moves[p->direction.d][0] * i;
	ranges->start_y = ranges->middle_y + moves[p->direction.d][1] * i;
	p->direction.d += 2;
	ranges->end_x = ranges->middle_x + moves[p->direction.d][0] * i;
	ranges->end_y = ranges->middle_y + moves[p->direction.d][1] * i;
	p->direction.d -= 1;
}
