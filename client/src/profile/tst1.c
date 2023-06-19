
#include "tst1.h"
#include "zap.h"

uint8_t	tst1_prio(profile_t *profile)
{
	(void)profile;
	return ((uint8_t)250);
}

int	tst1_init_cb(profile_t *profile)
{
	tst1_t *tst1_ctx = malloc(sizeof(tst1_t));
	int r = 0;
	if (!tst1_ctx)
		r = -1;
	else {
		profile->ctx = tst1_ctx;
		profile->fsm_cb = tst1_fsm;
		profile->prio_cb = tst1_prio;
		// profile->state = TST1_INIT_ID;
		profile->state = TST1_MOVE_D_COORDINATE;

		tst1_ctx->d_x=1;
		tst1_ctx->d_y=2;
		memset(&tst1_ctx->tgt, 0, sizeof(coord_t));
	}
	return (r);
}

// TODO FIX IT
int connect_nbr_cb(req_t *req)
{
	req->profile->state = TST1_MOVE_D_COORDINATE;
	return (0);
}

int 	tst1_fsm(profile_t *profile)
{
	int		r = 0;
	req_t		*req = NULL;
	tst1_t		*ctx = ((tst1_t*)profile->ctx);

	switch (profile->state)
	{

		case (TST1_MOVE_D_COORDINATE): {
			gettimeofday(&ctx->t_send, NULL);
			memcpy(&ctx->tgt, &profile->zap->coord, sizeof(coord_t));
			ctx->tgt.__x = ( (int)profile->zap->coord.__x + (int)ctx->d_x * ((ctx->t_send.tv_usec & 0x1) ? -1 : 1) ) % profile->zap->max_x;
			if (ctx->tgt.__x < 0)
				ctx->tgt.__x = profile->zap->max_x + ctx->tgt.__x;
			// ctx->tgt.__x += (ctx->d_x * ((ctx->t_send.tv_usec & 0x1) ? -1 : 1));
			ctx->tgt.__y = ( profile->zap->coord.__y + ctx->d_y ) % profile->zap->max_y;
			if (ctx->tgt.__y < 0)
				ctx->tgt.__y = profile->zap->max_y + ctx->tgt.__y;
#ifdef VERBOSE
			fprintf(stderr, " [ID=%d] TST1_MOVE: position: d_x=%d d_y=%d current={%d %d} tgt={%d %d} time=%ld\n",
					profile->zap->player.id,
					ctx->d_x,
					ctx->d_y,
					profile->zap->coord.__x,
					profile->zap->coord.__y,
					ctx->tgt.__x,
					ctx->tgt.__y,
					ctx->t_send.tv_usec);
#endif
			r = zap_move_coordinate(profile->zap, &ctx->tgt);
			profile->state = TST1_CHECK_COORDINATE;
			break ;
		}
		case (TST1_CHECK_COORDINATE): {
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);
			// enable timeout on this ? TODO 
			//fprintf(stderr, "[ID=%d] TST1_CHECK t=%8ld\n",
			//		profile->zap->player.id,
			//		tv.tv_usec);
			//if (tv.tv_sec - 4 > ctx->t_send.tv_sec) {
			//	fprintf(stderr, "[ID=%d] TIMEOUT tgt={%d %d} current={%d %d}\n",
			//		profile->zap->player.id,
			//		ctx->tgt.__x,
			//		ctx->tgt.__y,
			//		profile->zap->coord.__x,
			//		profile->zap->coord.__y);
			//	while (1)
			//		;
			//	// profile->state = TST1_MOVE_D_COORDINATE;
			//}
			if ((ctx->tgt.__x == profile->zap->coord.__x)
				&& (ctx->tgt.__y == profile->zap->coord.__y)) {
				profile->state = TST1_MOVE_D_COORDINATE;
#ifdef VERBOSE
				fprintf(stderr, "[ID=%d] TST1_CHECK MATCH\n",
					profile->zap->player.id);
#endif
			}
			break ;
		}
		case (TST1_INIT_ID): {
			// TODO FIX IT
			r = zap_queue_profile_cmd(profile->zap, profile, connect_nbr_cb, CMD_CONNECT_NBR);
			profile->state = TST1_WAIT;
			break ;
		}
		case (TST1_WAIT):
		default:
			break ;
	}
	return (r);
}
