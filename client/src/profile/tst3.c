

#include "tst3.h"
#include "zap.h"

uint8_t	check_broadcast_history(profile_t *profile)
{
	broadcast_t *bc = NULL;
	char message[255] = {0};
	uint8_t prio = 255;
	int i = snprintf(message, 256, "player_id %d comin",
			profile->zap->player.id + 1);
	list_for_each_entry(bc, &profile->zap->team.broadcast, lst) {
		if (bc->msg_len != 0)
		{
			if (!(profile->zap->player.id & 0x1)) { // even
				if (!memcmp(bc->msg, message, i)) {
					fprintf(stderr, "CHECK_BROADCAST [ID=%d] detect partner bc={id=%d dir=%d msg={%s}}\n", profile->zap->player.id, bc->id, bc->dir, bc->msg);
					profile->state = TST3_YELL;
					prio = 127;
					break ;
				}
			}
		}
	}
	return (prio);
}

int	broadcast_cb(req_t *req)
{
	fprintf(stderr, "%s: [ID=%d] broadcast done\n", __func__, req->zap->player.id);
	req->profile->state = TST3_YELL;
	// req->profile->state = TST3_IDLE;
	return (0);
}

uint8_t	tst3_prio(profile_t *profile)
{
	(void)profile;
	if ((profile->zap->player.id & 0x1)) { // odd
		if (profile->state != TST3_WAIT &&
			profile->zap->player.stuff.content[R_NOURRITURE] > 15) {
			profile->state = TST3_YELL;
			return (127);
		}
	}
	if (profile->state != TST3_WAIT) {
		return (check_broadcast_history(profile));
	}
	else {
		return (255);
	}
	// if (profile->state == TST3_WAIT) {
	// 	// check timestamp for time, know the tick server in some way
	// 	return (255);
	// }
	// else if (profile->zap->player.stuff.content[R_NOURRITURE] > 30) {
	// 	return (127); // take control
	// }
	// else {
	// 	return ((uint8_t)255);
	// }
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
			fprintf(stderr, "%s: [ID=%d] available for broadcast food=%d\n", __func__, profile->zap->player.id, profile->zap->player.stuff.content[R_NOURRITURE]);
			req_t *req = zap_get_req(profile->zap);
			if (!req) {
				fprintf(stderr, "[ID=%d] ERROR: out of req memory\n", profile->zap->player.id);
				return -1;
			}
			else {
				req->zap = profile->zap;
				req->profile = profile;
				req->cb = broadcast_cb;
				INIT_LIST_HEAD(&req->lst);
				int tgt = (profile->zap->player.id + 1) % 6;
				char message[256] = {0};
				if (!(profile->zap->player.id & 0x1)) { // even
					int i = snprintf(message, 256, "player_id %d come",
							tgt);
				}
				else { // odd
					int i = snprintf(message, 256, "player_id %d comin",
							profile->zap->player.id);
				}
				fprintf(stderr, "BROADCAST: [ID=%d] message={%s} pos={%d %d}\n",
						profile->zap->player.id,
						message, profile->zap->coord.__x, profile->zap->coord.__y);
				req->io_len = snprintf(req->buf, ZAP_RX_BUFSIZE, "%s %s", commands[CMD_BROADCAST].name,
					message);
				req->cmd_id = CMD_BROADCAST;
				zap_queue_reqlst(profile->zap, req);

				zap_queue_cmd(profile->zap, CMD_INVENTAIRE);

				profile->state = TST3_WAIT;
			}
			break ;
		}
		case (TST3_WAIT):
		case (TST3_IDLE):
		default:
			break ;
	}
	return (r);
}
