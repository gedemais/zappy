#include "zap.h"
#include "zap_req.h"
#include <errno.h>

/* alloc (remove elem from req_free) */
req_t	*zap_get_req(zap_t *zap)
{
	req_t *req = NULL;
	if (!list_empty(&zap->com.req_free))
	{
		req = list_first_entry(&zap->com.req_free, req_t, lst); // take it
		list_del(&req->lst);
		memset(req->buf, 0, 256);
		req->io_len = 0;
	}
	else {
		fprintf(stderr, "%s: memory error\n", __func__);
	}
	return (req);
}

/* req_send -> req_free */
void	zap_free_reqlst(zap_t *zap, req_t *req)
{
	list_del(&req->lst); // free the request
	list_add_tail(&req->lst, &zap->com.req_free); // free the request
	zap->com.count_send--;
#ifdef VERBOSE
	fprintf(stderr, "%s: [ID=%d] FREE req=%p lst=%p into req_free=%p\n", __func__, zap->player.id, req, &req->lst, &zap->com.req_free);
#endif
}
/* req_free -> req_queue (WARNING do not remove from req_free) */
void	zap_queue_reqlst(zap_t *zap, req_t *req)
{
	// list_del(&req->lst); // delete it from req_free
	list_add_tail(&req->lst, &zap->com.req_queue); // move it to req_queue
#ifdef VERBOSE
	fprintf(stderr, "%s: [ID=%d] QUEUE req=%p lst=%p into req_queue=%p\n", __func__, zap->player.id, req, &req->lst, &zap->com.req_queue);
#endif
}
void	zap_queue_reqlst_prepend(zap_t *zap, req_t *req)
{
	list_add(&req->lst, &zap->com.req_queue); // move it to req_queue
#ifdef VERBOSE
	fprintf(stderr, "%s: [ID=%d] QUEUE req=%p lst=%p into req_queue=%p\n", __func__, zap->player.id, req, &req->lst, &zap->com.req_queue);
#endif
}
/* req_queue -> req_send */
void	zap_send_reqlst(zap_t *zap, req_t *req)
{
	list_del(&req->lst); // delete it from req_queue
	list_add_tail(&req->lst, &zap->com.req_send); // put it in req_send
	zap->com.count_send++;
#ifdef VERBOSE
	fprintf(stderr, "%s: [ID=%d] SEND req=%p lst=%p into req_send=%p\n", __func__, zap->player.id, req, &req->lst, &zap->com.req_send);
#endif
}

static req_t *zap_build_queue_cmd(zap_t *zap, uint8_t cmd_id)
{
	req_t *req = zap_get_req(zap);
#ifdef VERBOSE
		fprintf(stderr, "%s: [ID=%d] QUEUE req=%p noprofile command={%s}\n", __func__,
			zap->player.id, req,
			commands[cmd_id].name);
#endif
	if (req)
	{
		req->zap = zap;
		req->profile = NULL;
		req->cb = commands[cmd_id].cb;
		memcpy(req->buf, commands[cmd_id].name, commands[cmd_id].len);
		req->io_len = commands[cmd_id].len;
		req->cmd_id = cmd_id;
	}
	return (req);
}

int	zap_queue_cmd(zap_t *zap, uint8_t cmd_id)
{
	req_t *req = NULL;
	int r = 0;

	if (!zap || cmd_id >= CMD_MAX) {
		r = -EINVAL;
	}
	else if (list_empty(&zap->com.req_free))
	{
		r = -ENOMEM;
		fprintf(stderr, "%s:%d memory error req_queue size = %ld req_send size = %ld\n", __func__, __LINE__, list_count_nodes(&zap->com.req_queue), list_count_nodes(&zap->com.req_send));
	}
	else
	{
		req = zap_build_queue_cmd(zap, cmd_id);
		if (req) {
			zap_queue_reqlst(zap, req);
		}
	}
	return (r);
}

int	zap_queue_cmd_prepend(zap_t *zap, uint8_t cmd_id)
{
	req_t *req = NULL;
	int r = 0;

	if (!zap || cmd_id >= CMD_MAX) {
		r = -EINVAL;
	}
	else if (list_empty(&zap->com.req_free))
	{
		r = -ENOMEM;
	}
	else
	{
		req = zap_build_queue_cmd(zap, cmd_id);
		if (req) {
			zap_queue_reqlst_prepend(zap, req);
		}
	}
	return (r);
}

int	zap_queue_profile_cmd(zap_t *zap, profile_t *profile, zap_req_cb_t cb, uint8_t cmd_id)
{
	req_t *req = NULL;
	int r = 0;

	if (!zap || !profile || cmd_id >= CMD_MAX) {
		r = -EINVAL;
	}
	else if (list_empty(&zap->com.req_free))
	{
		r = -ENOMEM;
	}
	else
	{
		req = list_first_entry(&zap->com.req_free, req_t, lst); // take it
#ifdef VERBOSE
		fprintf(stderr, "%s: [ID=%d] move req=%p lst=%p into req_queue=%p profile={%s} command={%s}\n", __func__,
			zap->player.id, req, &req->lst, &zap->com.req_queue,
			profile->name,
			commands[cmd_id].name);
#endif
		req->zap = zap;
		req->profile = profile;
		req->cb = cb;
		bzero(req->buf, 255);
		memcpy(req->buf, commands[cmd_id].name, commands[cmd_id].len);
		req->io_len = commands[cmd_id].len;
		req->cmd_id = cmd_id;
		zap_queue_reqlst(zap, req);
	}
	return (r);
}

int	zap_recv_req(zap_t *zap, req_t *req)
{
	int r = 0;

	if (!zap || req->cmd_id >= CMD_MAX) {
		r = -EINVAL;
	}
	else
	{
#ifdef VERBOSE
		struct timeval tv = {0};
		gettimeofday(&tv, NULL);
		fprintf(stderr, "%s: [ID=%d] RECV req=%p command={%s} send_at=%ld.%ld recv_at=%ld.%ld noprofile , execute global cb\n", __func__,
			zap->player.id,
			req,
			commands[req->cmd_id].name,
			req->tv_send.tv_sec,
			req->tv_send.tv_usec,
			tv.tv_sec,
			tv.tv_usec
			);
#endif
		r = commands[req->cmd_id].cb(req);
		if (r == 0 && req->profile) {
			r = req->cb(req); // execute cb associated with req 
		}
		if (r == 0) {
			// failed command stay in send list
			zap_free_reqlst(zap, req);
		}
	}
	return (r);
}

int	zap_send_req(zap_t *zap)
{
	char buf[1024] = {0}; // TODO use com.buf ?
	int r = 0;

	if (list_empty(&zap->com.req_queue) // while there is request to send
		||  zap->com.count_send >= MAX_SEND_REQ) // and we have space in req_send
	{
		r = -1;
	}
	else
	{
		req_t *req = list_first_entry(&zap->com.req_queue, req_t, lst);
		gettimeofday(&req->tv_send, NULL);
#ifdef VERBOSE
		fprintf(stderr, "%s: [ID=%d] SEND cmd={%s} tv.usec=%8ld\n", __func__, zap->player.id, req->buf, req->tv_send.tv_usec);
#endif
		memcpy(buf, req->buf, req->io_len);
		buf[req->io_len] = '\n';
		if ((r = send(zap->com.socket, req->buf, req->io_len + 1, 0)) >= 0
			&& r == (int)req->io_len + 1) {
			zap_send_reqlst(zap, req);
			r = 0;
		}
		else {
			perror("send");
		}
	}
	return (r);
}

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
