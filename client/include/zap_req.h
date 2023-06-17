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

// # include "zap.h"
# include "list.h"

/* Global struct commands and response definition
**	- callback
**	- struct
**	- enum
**	- global
*/

/* callback */
#include "zap_callback.h"
int	zap_receive_response(zap_t *zap);
typedef int		(*zap_cb_t)(zap_t *);
typedef int	(*zap_req_cb_t)(req_t *);

/* struct */
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

/* enum */
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

/* global */
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

/*
** Request definition
**	- struct 
**	- function 
*/

#define MAX_SEND_REQ 1
typedef struct zap_s zap_t;
typedef struct profile_s profile_t;

/*
** struct req for request
** req are managed in 3 linked list (send, queue, free)
** cmd_id is used to call callback of global struct commands
** cb of global struct cmd are used to update all zap ctx (map, vision, stuff, ...)
** profile may want to add a custom cb (for changing state, or anything) and ctx
*/
typedef struct 	req_s req_t;
typedef int	(*zap_req_cb_t)(req_t *);
typedef struct req_s
{
	zap_t		*zap;
	profile_t	*profile;
	zap_req_cb_t	cb;
	char		buf[1024];
	uint32_t	io_len;
	uint8_t		state;
	uint8_t		cmd_id;
	list_t		lst;
} req_t;

/* req_send -> req_free */
void	zap_free_reqlst(zap_t *zap, req_t *req);
/* req_free -> req_queue */
void	zap_queue_reqlst(zap_t *zap, req_t *req);
void	zap_queue_reqlst_prepend(zap_t *zap, req_t *req);
/* req_queue -> req_send */
void	zap_send_reqlst(zap_t *zap, req_t *req);

/* 
** zap_queue_cmd
** use cmd_id to build a request and queue it
** it only use standard_cb as it build all the req alone
** profile that want to use req->cb should not use it
*/
int	zap_queue_cmd(zap_t *zap, uint8_t cmd_id);
/* 
** zap_queue_cmd_prepend
** like zap_queue_cmd but put the request in front of the req list
** WARNING this could break the suite of commands, so be careful
*/
int	zap_queue_cmd_prepend(zap_t *zap, uint8_t cmd_id);

/* call commands.cb + execute req cb if any */
int	zap_recv_req(zap_t *zap, req_t *req);
/* effectively send the req using send(2) */
int	zap_send_req(zap_t *zap);

/* 
** zap_queue_profile_cmd
** use profile and use cmd_id to build a request and queue it
** it set the corresponding entry of cmd_id of profile cb
** profile that want to use req->cb should use it
*/
int	zap_queue_profile_cmd(zap_t *zap, profile_t *profile, uint8_t cmd_id);

// void	zap_com_req_free(zap_t *zap, req_t *req);
// req_t	*zap_com_req_alloc(zap_t *zap);


/*
** Global com management
*/
#define ZAP_RX_BUFSIZE 1024

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


req_t	*zap_get_req(zap_t *zap);
#endif

