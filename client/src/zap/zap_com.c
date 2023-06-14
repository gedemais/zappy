#include "zap.h"
#include "zap_com.h"
#include "zap_getopt.h"

int	zap_receive_response(zap_t *zap)
{
	// take the first req in the req list TODO timestamp
	int r = 0;
	req_t *req = NULL;
	
	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	if (!list_empty(&zap->com->req_send))
	{
		req = list_first_entry(&zap->com->req_send, req_t, lst);
		r = commands[req->cmd_id].cb(req);
#ifdef VERBOSE
		fprintf(stderr, "%s:%d executed global cb of id=%d f_ptr=%p r=%d\n", __func__, __LINE__, req->cmd_id, commands[req->cmd_id].cb, r);
#endif
		if (r == 0) {
			zap->com->buf_rx[zap->com->buf_rx_len - 1] = '\0';
			zap->com->buf_rx_len--;
			if (req) {
				r = req->cb(req);
#ifdef VERBOSE
				fprintf(stderr, "%s:%d executed req cb of req=%p req->cb=%p r=%d\n", __func__, __LINE__, req, req->cb, r);
#endif
				zap_com_free_req(zap, req);
			}
		}
	}
	else {
		fprintf(stderr, "%s: rsp received but req list empty\n", __func__);
		r = -1;
	}
	return (r);
}

static int	zap_com_send_req(zap_t *zap, req_t *req)
{
	int r = 0;
	int len = strlen(req->buf);
	// TODO save timestamp

#ifdef VERBOSE
	fprintf(stderr, "%s:%s:%d send %d bytes {%s} req->cb=%p req->profile=%p\n", __FILE__, __func__, __LINE__, len, req->buf, req->cb, req->profile);
#endif
	if (send(zap->com->socket, req->buf, len, 0) < 0) {
		perror("send");
		r = -1;
	}
	return (r);
}

int	zap_send(zap_t *zap)
{
	int r = 0;
	return (r);
}

int	zap_receive(zap_t *zap)
{
	int 			r = 0;
	fd_set			read_fd_set;
	struct timeval	timeout = {.tv_sec = 0, .tv_usec = 100};
	com_t			*com = zap->com;

	FD_ZERO(&read_fd_set);
	FD_SET(com->socket, &read_fd_set);
	if ((r = select(1024, &read_fd_set, NULL, NULL, &timeout)) >= 0) {
		if (r > 0) {
			// Data available
			if ((r = recv(com->socket, com->buf_rx, ZAP_RX_BUFSIZE, 0)) < 0) {
				perror("recv");
				r = -1;
			}
			com->buf_rx_len = r;
#ifdef VERBOSE
			fprintf(stderr, "recv len=%d buf={%s} \n", r, com->buf_rx);
#endif
			bool found = false;
			for (int i = 0 ; i < RSP_MAX ; i++) {
				if (!memcmp(response[i].name, com->buf_rx, response[i].len)) {
					r = response[i].cb(zap);
					found = true;
				}
			}
			if (!found) {
				fprintf(stderr, "%s:%d received %d unknow byte\n", __func__, __LINE__, com->buf_rx_len);
				r = -1;
			}
		}
#ifdef VERBOSE
		else {
			fprintf(stderr, "%s:%d nothing\n", __func__, __LINE__);
		}
#endif
	}
	else {
		perror("select");
	}
	return (r);
}


int	zap_com_init(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;
	com_t *com = malloc(sizeof(com_t));

	if (!com) {
		r = -1;
	}
	else {
		/* Init lists and set all to free */
		INIT_LIST_HEAD(&com->req_free);
		INIT_LIST_HEAD(&com->req_queue);
		INIT_LIST_HEAD(&com->req_send);
		for (int i = 0 ; i < MAX_REQ ; i++) {
			list_add_tail(&com->req[i].lst, &com->req_free);
		}
		zap->com = com;

		r = zap_com_connect(opt, zap);
#ifdef VERBOSE
		fprintf(stdout, "%s:%d connect=%d MAXREQ=%d zap->map->max_x=%d zap->map->max_y=%d\n",
			__func__,
			__LINE__,
			r,
			MAX_REQ,
			opt->max_x,
			opt->max_y);
#endif
	}
	return (r);
}

void	zap_com_deinit(zap_t *zap)
{
	free(zap->com);
}


