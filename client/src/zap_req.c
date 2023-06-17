#include "zap.h"
#include "zap_req.h"
#include <errno.h>

/* req_send -> req_free */
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
	return (req);
}

/* req_send -> req_free */
void	zap_free_reqlst(zap_t *zap, req_t *req)
{
	list_del(&req->lst); // free the request
	list_add_tail(&req->lst, &zap->com.req_free); // free the request
	zap->com.count_send--;
#ifdef VERBOSE
	fprintf(stderr, "%s: FREE move req=%p lst=%p into req_free=%p\n", __func__, req, &req->lst, &zap->com.req_free);
#endif
}
/* req_free -> req_queue */
void	zap_queue_reqlst(zap_t *zap, req_t *req)
{
	// list_del(&req->lst); // delete it from req_free
	list_add_tail(&req->lst, &zap->com.req_queue); // move it to req_queue
#ifdef VERBOSE
	fprintf(stderr, "%s: QUEUE move req=%p lst=%p into req_queue=%p\n", __func__, req, &req->lst, &zap->com.req_queue);
#endif
}
void	zap_queue_reqlst_prepend(zap_t *zap, req_t *req)
{
	list_add(&req->lst, &zap->com.req_queue); // move it to req_queue
#ifdef VERBOSE
	fprintf(stderr, "%s: QUEUE move req=%p lst=%p into req_queue=%p\n", __func__, req, &req->lst, &zap->com.req_queue);
#endif
}
/* req_queue -> req_send */
void	zap_send_reqlst(zap_t *zap, req_t *req)
{
	list_del(&req->lst); // delete it from req_queue
	list_add_tail(&req->lst, &zap->com.req_send); // put it in req_send
	zap->com.count_send++;
#ifdef VERBOSE
	fprintf(stderr, "%s: SEND move req=%p lst=%p into req_send=%p\n", __func__, req, &req->lst, &zap->com.req_send);
#endif
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
	}
	else
	{
		req = zap_get_req(zap);
#ifdef VERBOSE
		fprintf(stderr, "%s: QUEUE move req=%p noprofile command={%s}\n", __func__,
			req,
			commands[cmd_id].name);
#endif
		req->zap = zap;
		req->profile = NULL;
		req->cb = commands[cmd_id].cb;
		memcpy(req->buf, commands[cmd_id].name, commands[cmd_id].len);
		req->io_len = commands[cmd_id].len;
		req->cmd_id = cmd_id;
		zap_queue_reqlst(zap, req);
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
		req = zap_get_req(zap);
#ifdef VERBOSE
		fprintf(stderr, "%s: ==PREPEND=== WARNING QUEUE move req=%p noprofile command={%s}\n", __func__,
			req,
			commands[cmd_id].name);
#endif
		req->zap = zap;
		req->profile = NULL;
		req->cb = commands[cmd_id].cb;
		bzero(req->buf, 255);
		memcpy(req->buf, commands[cmd_id].name, commands[cmd_id].len);
		req->io_len = commands[cmd_id].len;
		req->cmd_id = cmd_id;
		zap_queue_reqlst_prepend(zap, req);
	}
	return (r);
}

int	zap_queue_profile_cmd(zap_t *zap, profile_t *profile, uint8_t cmd_id)
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
		fprintf(stderr, "%s: move req=%p lst=%p into req_queue=%p profile={%s} command={%s}\n", __func__,
			req, &req->lst, &zap->com.req_queue,
			profile->name,
			commands[cmd_id].name);
#endif
		req->zap = zap;
		req->profile = profile;
		req->cb = profile->req_cb[cmd_id];
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
		fprintf(stderr, "%s: FREE (response recv) move req=%p noprofile command={%s}, execute global cb\n", __func__,
			req,
			commands[req->cmd_id].name);
#endif
		r = commands[req->cmd_id].cb(req);
		if (r == 0 && req->profile) {
			r = req->cb(req); // execute cb associated with req 
		}
		zap_free_reqlst(zap, req);
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
#ifdef VERBOSE
		fprintf(stderr, "%s: SEND cmd={%s}\n", __func__, req->buf);
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
