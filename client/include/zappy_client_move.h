#ifndef ZAPPY_CLIENT_MOVE_H
# define ZAPPY_CLIENT_MOVE_H


# include <stdint.h>


/* this functions send the command to server and update internal struct
 * with new position/orientation */
int			zappy_client_set_orientation(zappy_client_t *client, uint8_t orientation);
int			zappy_client_move_front(zappy_client_t *client);
int			zappy_client_move_left(zappy_client_t *client);
int			zappy_client_move_right(zappy_client_t *client);
int			zappy_client_move(zappy_client_t *client, int relative_pos_target);


#endif
