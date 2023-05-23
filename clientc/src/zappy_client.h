#ifndef ZAPPY_CLIENT_H
# define ZAPPY_CLIENT_H

#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define CLIENT_BUFSIZE 4096

enum client_tasks  {
	SEARCH_FOOD,
	ELEVATION,
	REPRODUCE,
};

#define LINEMATE		0x01
#define DERAUMERE		0x02
#define SIBUR			0x04
#define MENDIANE		0x08
#define PHIRAS			0x10
#define THYSTAME		0x20

#define VISION_MAP_MAX 16

typedef struct zappy_client_s
{
	int					socket;
    struct sockaddr_in	sockaddr;
	uint8_t				buf[CLIENT_BUFSIZE];
	int					pos_x;
	int					pos_y;

	uint8_t				vision_map[VISION_MAP_MAX]; // Like in subject
	uint8_t				relative_pos; // Relative position for vision map

} zappy_client_t;

int zappy_client(zappy_client_opt_t *opt);

#endif
