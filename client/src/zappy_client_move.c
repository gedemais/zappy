
#include "zappy_client.h"

int			zappy_client_move_cb(zappy_client_t *client)
{
	int r = 0;

	if (memcmp(client->buf, "ok", strlen("ok")))
	{
		r = -1;
	}
	return (r);
}

int			zappy_client_set_orientation(zappy_client_t *client, uint8_t orientation)
{
	int r = 0;

	if (client->relative_orientation != orientation)
	{
		if (client->relative_orientation == ((orientation + 1) & 0x03))
		{
			r = zappy_client_transceive(client, turn_left_cmd,
								strlen(turn_left_cmd), zappy_client_move_cb);
		}
		else if (client->relative_orientation == ((orientation - 1) & 0x03))
		{
			r = zappy_client_transceive(client, turn_right_cmd,
								strlen(turn_right_cmd), zappy_client_move_cb);
		}
		else
		{
			r = zappy_client_transceive(client, turn_right_cmd,
								strlen(turn_right_cmd), zappy_client_move_cb);
			if (r == 0)
			{
				client->relative_orientation = ((client->relative_orientation + 1) & 0x3);
				r = zappy_client_transceive(client, turn_right_cmd,
									strlen(turn_right_cmd), zappy_client_move_cb);
			}
		}
		if (r == 0)
		{
			client->relative_orientation = orientation;
		}
	}
	return (r);
}

int			get_row_from_pos(int pos)
{
	int row = 0;

	for (int i = 0 ; i < 9 ; i++) {
		if (vision_row_start[i] <= pos && vision_row_start[i] + vision_row_size[i] > pos)
		{
			row = i;
			break ;
		}
	}
	return (row);
}

int			zappy_client_move_front(zappy_client_t *client)
{
	int r = 0;

	if (client->relative_orientation != ORIENTATION_FRONT)
	{
		r = zappy_client_set_orientation(client, ORIENTATION_FRONT);
	}
	if (r == 0)
	{
		r = zappy_client_transceive(client, advance_cmd, strlen(advance_cmd), zappy_client_move_cb);
	}
	if (r == 0)
	{
		int current_row = get_row_from_pos(client->relative_pos);
		client->relative_pos += vision_row_size[current_row] + 1;
	}
	return (r);
}

int			zappy_client_move_left(zappy_client_t *client)
{
	int r = 0;

	if (client->relative_orientation != ORIENTATION_LEFT)
	{
		r = zappy_client_set_orientation(client, ORIENTATION_LEFT);
	}
	if (r == 0)
	{
		r = zappy_client_transceive(client, advance_cmd, strlen(advance_cmd), zappy_client_move_cb);
	}
	if (r == 0)
	{
		client->relative_pos -= 1;
	}
	return (r);
}

int			zappy_client_move_right(zappy_client_t *client)
{
	int r = 0;

	if (client->relative_orientation != ORIENTATION_RIGHT)
	{
		r = zappy_client_set_orientation(client, ORIENTATION_RIGHT);
	}
	if (r == 0)
	{
		r = zappy_client_transceive(client, advance_cmd, strlen(advance_cmd), zappy_client_move_cb);
	}
	if (r == 0)
	{
		client->relative_pos += 1;
	}
	return (r);
}

/* main move function : it move the player to the
 * relative_pos_target.
 * */
int			zappy_client_move(zappy_client_t *client, int relative_pos_target)
{
	int r = 0;
	int target_row = 0;
	int current_row = 0;

	if (relative_pos_target > 80) {
		r = -1;
	}
	if (r == 0)
	{
		current_row = get_row_from_pos(client->relative_pos);
		target_row = get_row_from_pos(relative_pos_target);
		if (target_row > current_row)
		{
			r = zappy_client_move_front(client);
			if (r == 0)
			{
				current_row = get_row_from_pos(client->relative_pos);
			}
		}
		if (r == 0 && target_row < current_row)
		{
			fprintf(stderr, "can't move backward, error\n");
			r = -1;
		}
	}
	if (r == 0)
	{
		// Now we're in the same row as the target,
		// so cmp and move left or right
		if (client->relative_pos < relative_pos_target)
		{
			r = zappy_client_move_right(client);
		}
		if (r == 0 && client->relative_pos > relative_pos_target)
		{
			r = zappy_client_move_left(client);
		}
	}
	return (r);
}

