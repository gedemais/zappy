#include "main.h"

void	free_view(t_dynarray *view)
{
	for (int i = 0; i < view->nb_cells; i++)
		free_dynarray(dyacc(view, i));
	free_dynarray(view);
}

void	send_see_response(t_env *env, t_dynarray *view, t_player *p)
{
	t_tile	*tile;
	uint8_t	*loot;

	FLUSH_RESPONSE
	ft_strcat(env->buffers.response, "{");
	for (int i = 0; i < view->nb_cells; i++)
	{
		tile = dyacc(view, i);
		ft_strcat(env->buffers.response, (i > 0 && i < view->nb_cells) ? "," : "");
		for (int j = 0; j < tile->content.nb_cells; j++)
		{
			loot = dyacc(&tile->content, j);
			ft_strcat(env->buffers.response, (j > 0 && j < tile->content.nb_cells) ? " " : "");
			if ((int)*loot != 255)
				ft_strcat(env->buffers.response, loot_titles[(int)*loot]);
		}
	}
	ft_strcat(env->buffers.response, "}");
	response(env, p);
}

void	compute_view_ranges(t_env *env, t_view_ranges *ranges, t_player *p, uint8_t i)
{
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
