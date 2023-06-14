#include "zap.h"
#include "zap_com.h"
#include "zap_profile.h"
#include "tst.h"
#include "zap_getopt.h"

enum zap_r {
	ZAP_OK,
	ZAP_ERROR,
	PARSE_ERROR
};

int		zap_register_profile(zap_t *ctx, profile_t *profile)
{
	profile->zap = ctx;
	list_add(&profile->lst, &ctx->profile);
	int r = profile->init_cb(profile);
	fprintf(stderr, "adding profile to profile list r=%d\n", r);
	return (r);
}

void		zap_abs_droite(zap_t *zap)
{
	zap->coord.abs_direction = ((zap->coord.abs_direction + 4) % 16);
}
void		zap_abs_gauche(zap_t *zap)
{
	zap->coord.abs_direction = (zap->coord.abs_direction == 0 ? 12 : (zap->coord.abs_direction - 4) % 16);
}
void		zap_abs_avance(zap_t *zap)
{
	switch (zap->coord.abs_direction)
	{
		case (CARDINAL_W): {
			if (zap->coord.abs_pos.pos_x == 0) { zap->coord.abs_pos.pos_x = zap->max_x - 1; }
			else { zap->coord.abs_pos.pos_x--; }
			break ;
			} 
		case (CARDINAL_E): {
			if (zap->coord.abs_pos.pos_x == zap->max_x - 1) { zap->coord.abs_pos.pos_x = 0; }
			else { zap->coord.abs_pos.pos_x++; }
			break ;
			} 
		case (CARDINAL_S): {
			if (zap->coord.abs_pos.pos_y == zap->max_y - 1) { zap->coord.abs_pos.pos_y = 0; }
			else { zap->coord.abs_pos.pos_y++; }
			break ;
			} 
		case (CARDINAL_N): {
			if (zap->coord.abs_pos.pos_y == 0) { zap->coord.abs_pos.pos_y = zap->max_y - 1; }
			else { zap->coord.abs_pos.pos_y--; }
			break ;
			}
	}
}

static req_t	*zap_com_queue_req(zap_t *zap)
{
	req_t *req = NULL;
	if (!list_empty(&zap->com.req_free))
	{
		req = list_first_entry(&zap->com.req_free, req_t, lst); // take it
		list_del(&req->lst);
		list_add_tail(&req->lst, &zap->com.req_queue);
		fprintf(stderr, "%s: move req=%p lst=%p into req_queue=%p\n", __func__, req, &req->lst, &zap->com.req_queue);
	}
	else
	{
		fprintf(stderr, "%s error out of memory", __func__);
		req = NULL;
	}
	return (req);
}

int	zap_queue_cmd(zap_t *zap, uint8_t cmd_id)
{
	req_t *req = NULL;
	int r = 0;

	if (cmd_id < CMD_MAX) {
		req = zap_com_queue_req(zap);
		if (req) {
			req->zap = zap;
			req->profile = NULL;
			req->cb = commands[cmd_id].cb;
			memcpy(req->buf, commands[cmd_id].name, commands[cmd_id].len);
			req->io_len = commands[cmd_id].len;
			req->cmd_id = cmd_id;
		}
		else {
			r = -1;
		}
	}
	else {
		r = -1;
	}
	return (r);
}

static int	zap_com_tcp_connect(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;
	com_t *com = &zap->com;
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
    	if ((com->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		r = -1;
    	}
	if (r == 0)
	{
		fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
    		com->sockaddr.sin_family = AF_INET;
    		com->sockaddr.sin_port = htons(opt->server_port);
    		if (inet_pton(AF_INET, opt->server_addr, &com->sockaddr.sin_addr) <= 0) {
			perror("inet_pton");
			close(com->socket);
			r = -1;
    		}
	}
	if (r == 0)
	{
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
    		if (connect(com->socket, (struct sockaddr*)&com->sockaddr, sizeof(struct sockaddr_in)) < 0) {
			perror("connect");
			close(com->socket);
			r = -1;
    		}
		int flag = 1;
		// r = setsockopt(com->socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	}
	return (r);
}

static int	zap_com_zappy_connect(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;
	int len = 0;
	com_t *com = &zap->com;

		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	if ((r = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) < 0) {
		perror("recv");
		close(com->socket);
		r = -1;
	}
	if (r == 0)
	{
		fprintf(stderr, "%s:%d r=%d buf=%s\n", __func__, __LINE__, r, com->buf_rx);
		if (strcmp((char*)com->buf_rx, "BIENVENUE\n")) {
			fprintf(stderr, "%s: error BIENVENUE not recv\n", __func__);
			close(com->socket);
			r = -1;
		} else {
			fprintf(stderr, "%s----------\n", com->buf_rx);
		}
	}
	if (r == 0)
	{
		if (send(com->socket, opt->team_name, strlen(opt->team_name), 0) < 0) {
			perror("send");
			close(com->socket);
			r = -1;
		}
	}
	if (r == 0)
	{
		bzero(com->buf_rx, ZAP_RX_BUFSIZE);
		if ((len = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) < 0) {
			perror("recv");
			close(com->socket);
			r = -1;
		}
	}
	if (r == 0)
	{
		if (atoi((char*)com->buf_rx) < 1) {
			fprintf(stderr, "%s: max client connection reached\n", __func__);
			r = -1;
		}
	}
	if (r == 0)
	{
		for (int i = 0 ; com->buf_rx[i] ; i++) {
			if (com->buf_rx[i] == '\n') {
				opt->max_x = atoi((char*)&com->buf_rx[i+1]);
			}
			if (com->buf_rx[i] == ' ') {
				opt->max_y = atoi((char*)&com->buf_rx[i+1]);
				break ;
			}
		}
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
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		r = zap_com_tcp_connect(opt, zap);
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		if (r == 0) {
			r = zap_com_zappy_connect(opt, zap);
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		}
	}
	return (r);
}





static int zap_init(zap_opt_t *opt, zap_t **zap)
{
	int r = ZAP_OK;

	if (!(*zap = malloc(sizeof(zap_t)))) {
		r = ZAP_ERROR;
	}
	if (r == ZAP_OK) {
		INIT_LIST_HEAD(&(*zap)->com.req_free);
		INIT_LIST_HEAD(&(*zap)->com.req_queue);
		INIT_LIST_HEAD(&(*zap)->com.req_send);
		for (int i = 0 ; i < MAX_REQ ; i++) {
			list_add_tail(&(*zap)->com.req[i].lst, &(*zap)->com.req_free);
			fprintf(stderr, "req[%d}=%p lst=%p req_free=%p\n", i, &(*zap)->com.req[i], &(*zap)->com.req[i].lst, &(*zap)->com.req_free);
		}
		r = zap_com_connect(opt, *zap);
	}
	if (r == ZAP_OK) {
		INIT_LIST_HEAD(&(*zap)->profile);
	}
	if (r == ZAP_OK) {
		tst_init(*zap);
	}
	return (r);
}

static void zap_deinit(zap_t **zap)
{
	// zap_com_deinit(*zap);
	// zap_team_deinit(*zap);
	// zap_player_deinit(*zap);
	free(*zap);
}

int	zap_receive_response(zap_t *zap)
{
	// take the first req in the req list TODO timestamp
	int r = 0;
	req_t *req = NULL;
	
	// if there is a req waiting a resp in req_send
	if (!list_empty(&zap->com.req_send))
	{
		req = list_first_entry(&zap->com.req_send, req_t, lst); // take it
		r = commands[req->cmd_id].cb(req); // execute zap_cb for this given cmd id
						// func are in zap_callback.c
						// def of command array are in zap_def.h
						// this cb is used to update all zap context :
						// team, map, stuff
						// note that cb mort, deplacement, message do it too
						// so, zap ctx is updated before profile-specific cb
#ifdef VERBOSE
		fprintf(stderr, "%s:%d recv response of {%s} executed global callback cb_ptr=%p r=%d\n",
			__func__, __LINE__, commands[req->cmd_id].name, commands[req->cmd_id].cb, r);
#endif
		if (r == 0)
		{ // if everything normal
			zap->com.buf_rx[zap->com.buf_rx_len - 1] = '\0';
			zap->com.buf_rx_len--;
			if (req->profile) // if there is profile in req
			{
				r = req->cb(req); // execute cb associated with req 
					// this cb can be used to set fsm in profile
					// or set profile-specific ctx. profile can freely check
					// value in zap context (team, map, stuff) in this cb
#ifdef VERBOSE
				fprintf(stderr, "%s:%d executed request callback of req=%p req->cb=%p r=%d\n",
					__func__, __LINE__, req, req->cb, r);
			}
			else
			{
				fprintf(stderr, "%s:%d no-profile command response received\n", __func__, __LINE__);
#endif /* VERBOSE */
			}
		}
		if (r == 0)
		{ // if everything normal 
#ifdef VERBOSE
			fprintf(stderr, "%s:%d delete send request, move it to req_free\n", __func__, __LINE__);
#endif
			fprintf(stderr, "%s: move req=%p lst=%p into req_free=%p\n", __func__, req, &req->lst, &zap->com.req_free);
			zap->com.count_send--;
			list_del(&req->lst);
			list_add_tail(&req->lst, &zap->com.req_free); // free the request
		}
	}
	else {
		fprintf(stderr, "%s: error: rsp received but req list empty\n", __func__);
		r = -1;
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
int	zap_handler(zap_t *zap)
{
	int 			r = 0;
	fd_set			read_fd_set;
	struct timeval	timeout = {.tv_sec = 0, .tv_usec = 100};
	com_t			*com = &zap->com;
	profile_t *p = NULL;

	FD_ZERO(&read_fd_set);
	FD_SET(com->socket, &read_fd_set);
	if ((r = select(1024, &read_fd_set, NULL, NULL, &timeout)) < 0) {
		perror("select");
		r = -1;
	}
	if (r > 0) 
	{
		// FD available
		if ((r = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) < 0) {
			perror("recv");
			r = -1;
		}
		com->buf_rx_len = r;
	}
	if (r > 0)
	{
		// DATA available
#ifdef VERBOSE
		fprintf(stderr, "recv len=%d buf={%s} \n", r, com->buf_rx);
#endif
		// com->buf_rx_len = 0;
		// TODO if subsequent parsing fail the req shall not be removed
		bool found = false;
		for (int i = 0 ; i < RSP_MAX ; i++) {
			if (!memcmp(response[i].name, com->buf_rx, response[i].len)) // test first char of response
			{
				fprintf(stderr, "execute cb for response [%s]\n", response[i].name);
				r = response[i].cb(zap); // execute associated handler (zap_receive_response, mort, ..)
				found = true;
			}
		}
		if (!found) {
			fprintf(stderr, "%s:%d ERROR received %d unknow byte\n",
				__func__, __LINE__, com->buf_rx_len);
			r = -1;
		}
	}
#ifdef VERBOSE
	else {
		// no DATA
		// fprintf(stderr, "%s:%d nothing\n", __func__, __LINE__);
	}
#endif
	if (r == 0)
	{
		// once rsp is received, zap_cb executed, profile_cb executed
		// search for the next profile to run
		uint8_t prio = 255;
		profile_t *p = NULL;
		int i = 0;
		if (!list_empty(&zap->profile))
		{
			profile_t *head = NULL;
			list_for_each_entry(head, &zap->profile, lst) {
				uint8_t profile_prio = head->prio_cb(head);
				if (prio >= profile_prio) {
					prio = profile_prio;
					p = head;
				}
#ifdef VERBOSE
			//fprintf(stderr, "%s:%d profile(%d] prio=%hhu current_prio=%hhu\n",
			//	__func__, __LINE__, i++, profile_prio, prio);
#endif
			};
		}
		if (p)
		{
			r = p->fsm_cb(p); // run finite state machine of profile p
		}
	}
	if (r == 0)
	{
		char buf[1024] = {0};
		// now check if we can send some req
		while (!list_empty(&com->req_queue) // while there is request to send
			&& com->count_send < MAX_SEND_REQ) // and we have space in req_send
		{
			req_t *req = list_first_entry(&com->req_queue, req_t, lst);
			list_del(&req->lst); // delete it from req_queue
			list_add_tail(&req->lst, &com->req_send); // put it in req_send
			fprintf(stderr, "%s: move req=%p lst=%p into req_send=%p\n", __func__, req, &req->lst, &zap->com.req_send);
			// req->buf[req->io_len++] = '\n';
			memcpy(buf, req->buf, req->io_len);
			buf[req->io_len] = '\n';
			fprintf(stderr, "send req {%s} %d req waiting for rsp req->io_len=%d\n", buf, com->count_send, req->io_len);
			if (send(com->socket, req->buf, req->io_len + 1, 0) < 0) { // and send it.
				perror("send");
			}
			usleep(1);
			// usleep(1000000);
			com->count_send++;
		}
	}
	return (r);
}

static void zap_loop(zap_t *zap)
{
	int r = 0;
	while (1)
	{
		r = zap_handler(zap);
		if (r != ZAP_OK) {
			fprintf(stderr, "%s: top level error:%d", __func__, r);
		}
	}
}

int		zap(zap_opt_t *opt)
{
	int r = 0;
	zap_t *zap;

	if (!opt)
	{
		r = -1;
	}
	else
	{
		r = zap_init(opt, &zap);
		if (r == 0) {
			zap_loop(zap);
		}
	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		zap_deinit(&zap);
	}
	return (r);
}
