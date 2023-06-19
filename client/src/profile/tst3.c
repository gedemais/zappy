

#include "tst3.h"
#include "zap.h"

int	broadcast_cb(req_t *req)
{
	req->profile->state = TST3_YELL;
	return (0);
}

uint8_t	tst3_prio(profile_t *profile)
{
	(void)profile;
	// TODO LMA : we have to determine the numlber of food we have, for that we shall
	// get the server time in some way and decrement the food accordingly
	// if we have food it return 200
	if (profile->state == TST3_YELL && profile->zap->player.stuff.content[R_NOURRITURE] > 20) {
		// fprintf(stderr, "%s: [ID=%d] we have %d food, yelling\n", __func__, profile->zap->player.id, profile->zap->player.stuff.content[R_NOURRITURE]);
		return (255);
	}
	else {
	//	fprintf(stderr, "%s: we have %d food, not yelling \n", __func__, profile->zap->player.stuff.content[R_NOURRITURE]);
	}
	// otherwise 255
	return ((uint8_t)255);
}

int	tst3_init_cb(profile_t *profile)
{
	tst3_t *tst3_ctx = malloc(sizeof(tst3_t));
	int r = 0;
	if (!tst3_ctx)
		r = -1;
	else {
		profile->ctx = tst3_ctx;
		profile->fsm_cb = tst3_fsm;
		profile->prio_cb = tst3_prio;
		profile->state = TST3_YELL;
	}
	return (r);
}

int 	tst3_fsm(profile_t *profile)
{
	int		r = 0;
	req_t		*req = NULL;

	switch (profile->state)
	{
		case (TST3_YELL): {
			fprintf(stderr, "%s YELLING\n", __func__);
			req_t *req = zap_get_req(profile->zap);
			if (!req) {
				return -1;
			}
			req->zap = profile->zap;
			req->profile = profile;
			req->cb = broadcast_cb;
			INIT_LIST_HEAD(&req->lst);
			req->io_len = snprintf(req->buf, ZAP_RX_BUFSIZE, "%s %s", commands[CMD_BROADCAST].name,
				"hello!!");
			req->cmd_id = CMD_BROADCAST;
			zap_queue_reqlst_prepend(profile->zap, req);
			profile->state = TST3_WAIT;
			break ;
		}
		case (TST3_WAIT):
		default:
			break ;
	}
	return (r);
}
