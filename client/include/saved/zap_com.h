#ifndef ZAP_COM_H
# define ZAP_COM_H

# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <stdio.h>

# include "zap.h"
# include "list.h"

#include "zap_callback.h"

/*
** req are managed in 2 linked list (free, alloc)
*/
#define MAX_REQ 1
typedef struct req_s req_t;
typedef int		(*zap_req_cb_t)(req_t *);

typedef struct zap_s zap_t;
typedef struct profile_s profile_t;
typedef struct req_s
{
	zap_t		*zap;
	profile_t	*profile;
	char		buf[1024];
	uint32_t	io_len;
	zap_req_cb_t	cb;
	uint8_t		state;
	uint8_t		cmd_id;
	list_t		lst;
} req_t;

void	zap_com_req_free(zap_t *zap, req_t *req);
req_t	*zap_com_req_alloc(zap_t *zap);
int	zap_receive_response(zap_t *zap);


#define ZAP_RX_BUFSIZE 1024
#define MAX_SEND_REQ 1

typedef struct com_s
{
	int socket;
	req_t req[MAX_SEND_REQ + 25];
	struct sockaddr_in sockaddr;
	char buf_rx[ZAP_RX_BUFSIZE];
	int buf_rx_len;
	// TODO its not alloc free , its sended / queued / free
	// when recv it automatically send the next available queued, if any
	
	// list_t req_alloc;
	// list_t req_free;

	uint8_t count_send;
	list_t req_send;
	list_t req_queue;
	list_t req_free;
} com_t;

typedef int		(*zap_cb_t)(zap_t *);

typedef struct	s_cmd
{
	char	name[256];
	uint8_t	len;
	zap_req_cb_t  cb;
}				t_cmd;

typedef struct	t_rsp
{
	char	name[256];
	uint8_t	len;
	zap_cb_t  cb;
}		t_rsp;

enum			e_commands
{
	CMD_AVANCE,
	CMD_DROITE,
	CMD_GAUCHE,
	CMD_VOIR,
	CMD_INVENTAIRE,
	CMD_PREND,
	CMD_POSE,
	CMD_EXPULSE,
	CMD_BROADCAST,
	CMD_INCANTATION,
	CMD_FORK,
	CMD_CONNECT_NBR,
	CMD_MAX
};

enum			e_response
{
	RSP_OK,
	RSP_KO,
	RSP_LIST,
	RSP_ELEVATION,
	RSP_VALUE,
	RSP_MORT,
	RSP_DEPLACEMENT,
	RSP_MESSAGE,
	RSP_MAX		
};

static t_cmd	commands[CMD_MAX] = {
	[CMD_AVANCE]		= {.name = "avance", .len = strlen("avance"), .cb = zap_avance_cb},
	[CMD_DROITE]		= {.name = "droite", .len = strlen("droite"), .cb = zap_droite_cb},
	[CMD_GAUCHE]		= {.name = "gauche", .len = strlen("gauche"), .cb = zap_gauche_cb},
	[CMD_VOIR]		= {.name = "voir", .len = strlen("voir"), .cb = zap_voir_cb},
	[CMD_INVENTAIRE]	= {.name = "inventaire", .len = strlen("inventaire"), .cb = zap_inventaire_cb},
	[CMD_PREND]		= {.name = "prend", .len = strlen("prend"), .cb = zap_prend_cb},
	[CMD_POSE]		= {.name = "pose", .len = strlen("pose"), .cb = zap_pose_cb},
	[CMD_EXPULSE]		= {.name = "expulse", .len = strlen("expulse"), .cb = zap_expulse_cb},
	[CMD_BROADCAST]		= {.name = "broadcast", .len = strlen("broadcast"), .cb = zap_broadcast_cb},
	[CMD_INCANTATION]	= {.name = "incantation", .len = strlen("incantation"), .cb = zap_incantation_cb},
	[CMD_FORK]		= {.name = "fork", .len = strlen("fork"), .cb = zap_fork_cb},
	[CMD_CONNECT_NBR]	= {.name = "connect_nbr", .len = strlen("connect_nbr"), .cb = zap_connect_nbr_cb}
};

static t_rsp	response[CMD_MAX] = {
	 [RSP_OK]                                        = {.name = "ok", .len = strlen("ok"), .cb = zap_receive_response},
	 [RSP_KO]                                        = {.name = "ko", .len = strlen("ko"), .cb = zap_receive_response},
	 [RSP_LIST]                                      = {.name = "{", .len = strlen("{"), .cb = zap_receive_response},
	 [RSP_ELEVATION]                                 = {.name = "niveau actuel :", .len = strlen("niveau actuel "), .cb = zap_niveau_cb},
	 [RSP_VALUE]                                     = {.name = "TODO", .len = strlen("TODO"), .cb = zap_receive_response},
	 [RSP_MORT]                                      = {.name = "mort", .len = strlen("mort"), .cb = zap_mort_cb},
	 [RSP_DEPLACEMENT]                               = {.name = "deplacement", .len = strlen("deplacement"), .cb = zap_deplacement_cb},
	 [RSP_MESSAGE]                                   = {.name = "message", .len = strlen("message"), .cb = zap_message_cb},
};

typedef struct zap_s zap_t;
typedef struct zap_opt_s zap_opt_t;

int	zap_com_init(zap_opt_t *opt, zap_t *zap);
void	zap_com_deinit(zap_t *zap);
int	zap_com_connect(zap_opt_t *opt, zap_t *zap);
// int	zap_com_zappy_connect(zap_opt_t *opt, zap_t *zap);
// int	zap_com_tcp_connect(zap_opt_t *opt, zap_t *zap);
int	zap_receive(zap_t *zap);
int	zap_send_req(zap_t *zap, req_t *req);

int	zap_send(zap_t *zap);
int	zap_queue_cmd(zap_t *zap, uint8_t cmd_id);

#endif
