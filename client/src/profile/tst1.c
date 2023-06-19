
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

		tst1_ctx->d_x=2;
		tst1_ctx->d_y=3;
		memset(&tst1_ctx->tgt, 0, sizeof(coord_t));
	}
	return (r);
}

int connect_nbr_cb(req_t *req)
{
	req->profile->state = TST1_MOVE_REL_COORD;
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
			fprintf(stderr, "tgt = {%d %d}\n", ctx->tgt.__x, ctx->tgt.__y);
			ctx->tgt.__x = ( (int)profile->zap->coord.__x + (int)ctx->d_x * ((ctx->t_send.tv_usec & 0x1) ? -1 : 1) ) % profile->zap->max_x;
			fprintf(stderr, "tgt = {%d %d}\n", ctx->tgt.__x, ctx->tgt.__y);
			if (ctx->tgt.__x < 0)
				ctx->tgt.__x = profile->zap->max_x + ctx->tgt.__x;
			// ctx->tgt.__x += (ctx->d_x * ((ctx->t_send.tv_usec & 0x1) ? -1 : 1));
			fprintf(stderr, "tgt = {%d %d}\n", ctx->tgt.__x, ctx->tgt.__y);
			ctx->tgt.__y = ( profile->zap->coord.__y + ctx->d_y ) % profile->zap->max_y;
			fprintf(stderr, "tgt = {%d %d}\n", ctx->tgt.__x, ctx->tgt.__y);
			if (ctx->tgt.__y < 0)
				ctx->tgt.__y = profile->zap->max_y + ctx->tgt.__y;
			fprintf(stderr, "tgt = {%d %d}\n", ctx->tgt.__x, ctx->tgt.__y);
			fprintf(stderr, " [ID=%d] TST1_MOVE: position: d_x=%d d_y=%d current={%d %d} tgt={%d %d} time=%ld\n",
					profile->zap->player.id,
					ctx->d_x,
					ctx->d_y,
					profile->zap->coord.__x,
					profile->zap->coord.__y,
					ctx->tgt.__x,
					ctx->tgt.__y,
					ctx->t_send.tv_usec);
			r = zap_move_coordinate(profile->zap, &ctx->tgt);
			profile->state = TST1_CHECK_COORDINATE;
			break ;
		}
		case (TST1_CHECK_COORDINATE): {
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);
			//fprintf(stderr, "[ID=%d] TST1_CHECK t=%8ld\n",
			//		profile->zap->player.id,
			//		tv.tv_usec);
			if (tv.tv_sec - 4 > ctx->t_send.tv_sec) {
				fprintf(stderr, "[ID=%d] TIMEOUT tgt={%d %d} current={%d %d}\n",
					profile->zap->player.id,
					ctx->tgt.__x,
					ctx->tgt.__y,
					profile->zap->coord.__x,
					profile->zap->coord.__y);
				while (1)
					;
				// profile->state = TST1_MOVE_D_COORDINATE;
			}
			if ((ctx->tgt.__x == profile->zap->coord.__x)
				&& (ctx->tgt.__y == profile->zap->coord.__y)) {
				profile->state = TST1_MOVE_D_COORDINATE;
				fprintf(stderr, "[ID=%d] TST1_CHECK MATCH\n",
					profile->zap->player.id);
			}
			break ;
		}
		case (TST1_INIT_ID): {
			r = zap_queue_profile_cmd(profile->zap, profile, connect_nbr_cb, CMD_CONNECT_NBR);
			profile->state = TST1_WAIT;
			break ;
		}
		case (TST1_OLDMOVE_REL_COORD): {
			struct timeval tv = {0};
			gettimeofday(&((tst1_t*)profile->ctx)->t_send, NULL);
			((tst1_t*)profile->ctx)->tgt_dir =  1;
			// ((tst1_t*)profile->ctx)->tgt_dir =  tv.tv_usec & 0x1 ? -1 : 1;
			
			((tst1_t*)profile->ctx)->target_rel_coord_x *= ((tst1_t*)profile->ctx)->tgt_dir;

			r = zap_move_rel_coord(profile->zap,
				((tst1_t*)profile->ctx)->target_rel_coord_x,
				((tst1_t*)profile->ctx)->target_rel_coord_y);
#ifdef VERBOSE
			fprintf(stderr, "TST1_MOVE_REL_COORD [ID=%d] abs_pos_x=%d abs_pos_y=%d abs_dir=%d\n", profile->zap->player.id, profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y, profile->zap->coord.abs_direction);
			fprintf(stderr, "%s:%d move_rel_coord return move to x=%d y=%d r=%d\n", __func__, __LINE__, ((tst1_t*)profile->ctx)->target_rel_coord_x, ((tst1_t*)profile->ctx)->target_rel_coord_y, r);
#endif
			if (r == 0) {
				profile->state = TST1_CHECK;
			}
			break ;
		}
		case (TST1_CHECK): {
			int x, y, dir = 0;
			// dir = ((tst1_t*)profile->ctx)->tgt_dir == -1 ?
			// 	(((tst1_t*)profile->ctx)->init_direction - 4) % 16 :
			// 	((tst1_t*)profile->ctx)->init_direction;
			// dir = ((tst1_t*)profile->ctx)->init_direction;
			if (dir == CARDINAL_N) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)- ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)+ ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else if (dir == CARDINAL_E) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)+ ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)+ ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else if (dir == CARDINAL_W) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)- ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)- ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)+ ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)- ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			if (x < 0) {
				x += profile->zap->max_x;
			}
			if (y < 0) {
				y += profile->zap->max_y;
			}
			x %= profile->zap->max_x;
			y %= profile->zap->max_y;

#ifdef VERBOSE
			fprintf(stderr, "%s:%d TST1_CHECK [ID=%d] dir=%d abs_pos_x=%d abs_pos_y=%d abs_dir=%d init_dir=%d init_x=%d init_y=%d tgt_x=%d tgt_y=%d x=%d y=%d\n",
				__func__, 
				__LINE__, 
				profile->zap->player.id,
				dir,
				profile->zap->coord.abs_pos.pos_x,
				profile->zap->coord.abs_pos.pos_y,
				profile->zap->coord.abs_direction,
				((tst1_t*)profile->ctx)->init_pos_x,
				((tst1_t*)profile->ctx)->init_pos_y,
				((tst1_t*)profile->ctx)->target_rel_coord_x,
				((tst1_t*)profile->ctx)->target_rel_coord_y,
				((tst1_t*)profile->ctx)->init_direction, x, y);
#endif
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);
			if (((tst1_t*)profile->ctx)->t_send.tv_usec < tv.tv_usec - 4000 &&
				list_empty(&profile->zap->com.req_queue)) {
				// timeout, relaunch
				fprintf(stderr, "TIMEOUT !! \n");
				if (!list_empty(&profile->zap->com.req_send)) {
					req_t *head = NULL;
					int i = 0;
					struct timeval tv = {0};
					gettimeofday(&tv, NULL);
					fprintf(stderr, "== WARNING == req_send not empty ? current tv=%8ld\n", tv.tv_usec);
					list_for_each_entry(head, &profile->zap->com.req_send, lst) {	
						fprintf(stderr, "== WARNING == sended command dropped ? id=%d command={%s} tv_send=%8ld\n", i++, head->buf, head->tv_send.tv_usec);
					}
				}
				r = zap_queue_cmd(profile->zap, CMD_INVENTAIRE);
				profile->state = TST1_MOVE_REL_COORD;
			}
			if (profile->zap->coord.abs_pos.pos_x == x
				&& profile->zap->coord.abs_pos.pos_y == y)
			{
#ifdef VERBOSE
				fprintf(stderr, "%s: REACHED!! tgt_x=%d tgt_y=%d\n", __func__, ((tst1_t*)profile->ctx)->target_rel_coord_x, ((tst1_t*)profile->ctx)->target_rel_coord_y);
				((tst1_t*)profile->ctx)->init_direction = profile->zap->coord.abs_direction;
				((tst1_t*)profile->ctx)->init_pos_x = profile->zap->coord.abs_pos.pos_x;
				((tst1_t*)profile->ctx)->init_pos_y = profile->zap->coord.abs_pos.pos_y;
				((tst1_t*)profile->ctx)->target_rel_coord_x = (((tst1_t*)profile->ctx)->target_rel_coord_x + 1) % 4;
				((tst1_t*)profile->ctx)->target_rel_coord_y = (((tst1_t*)profile->ctx)->target_rel_coord_y + 1) % 4;
			struct timeval tv = {0};
			gettimeofday(&tv, NULL);
				((tst1_t*)profile->ctx)->target_rel_coord_x =  2 + tv.tv_usec % 3;
				((tst1_t*)profile->ctx)->target_rel_coord_y = 2 + tv.tv_usec % 3;
			
#endif
				r = zap_queue_cmd(profile->zap, CMD_INVENTAIRE);
				profile->state = TST1_MOVE_REL_COORD;
				
			}
			break ;
		}
		case (TST1_WAIT):
		default:
			break ;
	}
	return (r);
}
