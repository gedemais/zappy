#include "zap.h"
#include "tst1.h"
#include "zap_getopt.h"
#include "manager.h"

#include <errno.h>

// TODO launch 3 instance of zap and call 3 receive, 3 loop etc
// fix server input with a setsockopt TCP_NODELAY

enum zap_r {
	ZAP_OK,
	ZAP_ERROR,
	PARSE_ERROR
};

// TODO : team shared map

static int	zap_com_tcp_connect(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;
	com_t *com = &zap->com;

    	if ((com->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		r = -1;
    	}
	if (r == 0)
	{
		// int flag = 1;
		// r = setsockopt(com->socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    		com->sockaddr.sin_family = AF_INET;
    		com->sockaddr.sin_port = htons(opt->server_port);
    		if (inet_pton(AF_INET, opt->server_addr, &com->sockaddr.sin_addr) <= 0) {
			perror("inet_pton");
			close(com->socket);
			r = -1;
    		}
		fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
	}
	if (r == 0)
	{
    		if (connect(com->socket, (struct sockaddr*)&com->sockaddr, sizeof(struct sockaddr_in)) < 0) {
			perror("connect");
			close(com->socket);
			r = -1;
    		}
	}
	return (r);
}

static int	zap_com_zappy_connect(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;
	int len = 0;
	com_t *com = &zap->com;

	if ((len = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0))
		&& !strcmp((char*)com->buf_rx, "BIENVENUE\n")
		&& send(com->socket, opt->team_name, strlen(opt->team_name), 0) > 0
		&& (len = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) > 0)
	{
	fprintf(stderr, "%s:%d r=%d buf={%s}\n", __func__, __LINE__, r, com->buf_rx);
		/* parse "\n%d %d"*/
		if ((zap->player.id = atoi((char*)com->buf_rx)) < 1) {
			fprintf(stderr, "%s: max client connection reached\n", __func__);
			r = -1;
		}
		for (int i = 0 ; i < len ; i++) {
			if (com->buf_rx[i] == '\n') {
				zap->max_x = atoi((char*)&com->buf_rx[i+1]);
			}
			if (com->buf_rx[i] == ' ') {
				zap->max_y = atoi((char*)&com->buf_rx[i+1]);
				break ;
			}
		}
	}
	else
	{
		fprintf(stderr, "%s: error\n", __func__);
		close(com->socket);
		r = -1;
	}
	if (r == 0)
	{
	}
	return (r);
}

int	zap_com_connect(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;

	if (!opt || !zap || !&zap->com) {
		r = -1;
	}
	else
	{
		com_t *com = &zap->com;
		r = zap_com_tcp_connect(opt, zap);
		if (r == 0) {
			r = zap_com_zappy_connect(opt, zap);
			fprintf(stderr, "%s:%d r=%d player_id=%d\n\n", __func__, __LINE__, r,
			zap->player.id);
		}
	}
	return (r);
}


static int zap_init(zap_opt_t *opt, zap_t **zap)
{
	int r = ZAP_OK;

	if (!(*zap = malloc(sizeof(zap_t) * opt->instance))) {
		r = ZAP_ERROR;
	}
	if (r == ZAP_OK) {
		fprintf(stderr, "opt->Winstances=%d\n", opt->instance);
		for (int j = 0 ; j < opt->instance ; j++) {
			INIT_LIST_HEAD(&(*zap)[j].com.req_free);
			INIT_LIST_HEAD(&(*zap)[j].com.req_queue);
			INIT_LIST_HEAD(&(*zap)[j].com.req_send);
			for (int i = 0 ; i < MAX_SEND_REQ + 25; i++) {
				list_add_tail(&(*zap)[j].com.req[i].lst, &(*zap)[j].com.req_free);
				fprintf(stderr, "INIT req[%d}=%p lst=%p req_free=%p\n",
				i, &(*zap)[j].com.req[i], &(*zap)[j].com.req[i].lst, &(*zap)[j].com.req_free);
			}
			memset(&(*zap)[j].coord, 0, sizeof(coord_t));
			r = zap_com_connect(opt, &(*zap)[j]);
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
			INIT_LIST_HEAD(&(*zap)[j].profile);
			INIT_LIST_HEAD(&(*zap)[j].team.broadcast);
			INIT_LIST_HEAD(&(*zap)[j].team.broadcast_free);
			for (int i = 0 ; i < MAX_BROADCAST ; i++) {
				list_add_tail(&(*zap)[j].team.broadcast_history[i].lst, &(*zap)[j].team.broadcast_free);
			}
			(*zap)[j].vision.enabled = true;
			(*zap)[j].time = 0;
			(*zap)[j].player.alive = true;
		}
	}
	return (r);
}

static void zap_deinit(zap_opt_t *opt, zap_t **zap)
{
	profile_t *p = NULL;
	for (int i = 0 ; i < opt->instance ; i++)
	{
		list_for_each_entry(p, &(*zap)->profile, lst) {
			free(p);
		}
		free(*zap);
	}
}

int	zap_receive_response(zap_t *zap)
{
	// take the first req in the req list TODO timestamp
	int r = 0;
	req_t *req = NULL;
	
	// TODO parse input
	// TODO search in req for this response ? is response may be unsequential ?
	// fprintf(stderr, "%s:%d buf={%s}\n", __func__, __LINE__, zap->com.buf_rx);
	if (list_empty(&zap->com.req_send)) {
		fprintf(stderr, "buf_rx={%s} len=%d response received while req_send empty, fatal, abort\n", zap->com.buf_rx, zap->com.buf_rx_len);
		exit(1);
	}
	else
	{
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		req = list_first_entry(&zap->com.req_send, req_t, lst); // take it
		r = zap_recv_req(zap, req);
	}
	
	return (r);
}

// parse input :
// all possible first char of response are determined 
// So response are check against {"ok", "ko", "{", "mort", ...}
// At each response is associated a callback :
// 	- All response string  point to zap_receive_response
//		(ko, "{", ...)
// 	- All server message string point directly to specific callback
//		(mort, deplacement, ...)

int	zap_req_timeout(zap_t *zap)
{
	int r = 0;
	if (!list_empty(&zap->com.req_send))
	{
		req_t *req = list_first_entry(&zap->com.req_send, req_t, lst);
		struct timeval tv = {0};
		gettimeofday(&tv, NULL);
//		if (tv.tv_sec > req->tv_send.tv_sec+2) {
		if (tv.tv_sec > req->tv_send.tv_sec+2 ||
		(tv.tv_sec > req->tv_send.tv_sec && ((tv.tv_usec > req->tv_send.tv_usec) && (tv.tv_usec - req->tv_send.tv_usec > 1000000)))) {
			fprintf(stderr, "%s TIMEOUT no response [ID=%d] req=%p t_send=%ld.%ld curt=%ld.%ld resend\n", __func__,
						zap->player.id, req, req->tv_send.tv_sec, req->tv_send.tv_usec, tv.tv_sec, tv.tv_usec);
			// while (1)
			// 	;
			gettimeofday(&req->tv_send, NULL);
			req->buf[req->io_len] = '\n';
			if ((r = send(zap->com.socket, req->buf, req->io_len + 1, 0)) >= 0) {
				r = 0;
			}
			else {
				perror("send");
			}
			
		}
	}
	return (r);
}

int	zap_handler_input(zap_t *zap)
{
	int 			r = 0;
	fd_set			read_fd_set;
	struct timeval	timeout = {.tv_sec = 0, .tv_usec = 100};
	com_t			*com = &zap->com;

	FD_ZERO(&read_fd_set);
	FD_SET(com->socket, &read_fd_set);
	if ((r = select(1024, &read_fd_set, NULL, NULL, &timeout)) < 0) {
		perror("select");
		r = -1;
	}
	else if (r > 0) 
	{
		// FD available
		if ((r = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) < 0) {
			perror("recv");
			r = -1;
		}
		com->buf_rx_len = r;
	}
	else {
		r = zap_req_timeout(zap);
	}
	if (r > 0)
	{
		r = 0;
		// TODO if subsequent parsing fail the req shall not be removed
		bool found = false;
		// test first char of response
		for (int i = 0 ; i < RSP_MAX ; i++) {
			if (!memcmp(response[i].name, com->buf_rx, response[i].len))
			{
				r = response[i].cb(zap); // execute associated handler (zap_receive_response, mort, ..)
				found = true;
				if (r != 0) {
					fprintf(stderr, "%s: [ERROR] commands cb r=%d non fatal\n", __func__, r);
					r = 0;
				}
			}
		}
		if (!found) {
			// TODO LMA : connect_nbr
			if (com->buf_rx[0] >= '0' && com->buf_rx[0] <= '9') {
				zap->player.id = atoi(com->buf_rx);
				r = 0;
			}
			else {
				fprintf(stderr, "%s:%d ERROR received %d unknow byte\n",
					__func__, __LINE__, com->buf_rx_len);
				r = -1;
			}
		}
	}
	return (r);
}
int	zap_handler(zap_t *zap)
{
	int 			r = 0;

	r = zap_handler_input(zap);
	if (r == 0)
	{
		// once rsp is received, zap_cb executed, profile_cb executed
		// search for the next profile to run
		profile_t *p = zap_get_next_profile(zap);;
		if (p)
		{
			r = p->fsm_cb(p); // run finite state machine of profile p
		}
		if (r == 0)
		{
			char buf[1024] = {0};
			// now check if we can send some req
			if (list_empty(&zap->com.req_queue)) {
				// fprintf(stderr, "%s: req_list empty\n", __func__);
			}
			while (!list_empty(&zap->com.req_queue) // while there is request to send
				&& zap->com.count_send < MAX_SEND_REQ) // and we have space in req_send
			{
				r = zap_send_req(zap);
			}
		}
	}
	else {
		fprintf(stderr, "%s: [ERROR]\n", __func__);
	}
	return (r);
}

int	zap_profile_manager_init(zap_t *zap);

int		zap(zap_opt_t *opt)
{
	int r = 0;
	zap_t *zaps = NULL;

	r = zap_init(opt, &zaps);
	for (int i = 0 ; r == 0 && i < opt->instance ; i++) {
		r = zap_profile_manager_init(&zaps[i]);
	}
	while (r == 0)
	{
		bool alive = false;
		for (int i = 0 ; r == 0 && i < opt->instance ; i++) {
			if (zaps[i].player.alive) {
				r = zap_handler(&zaps[i]);
				alive = true;
			}
		}
		if (!alive)
		{
			r = 1;
		}
	}
	zap_deinit(opt, &zaps);
	return (r);
}
