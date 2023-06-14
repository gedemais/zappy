#include "farmer.h"
#include "zap_profile.h"
#include <stdbool.h>

// TODO FARMER DONT NEED A MAP SNAPSHOT
// IT ONLY USE THE VISION STRUCT FOR RELATIVE MOVEMENT

uint8_t	farmer_prio(profile_t *profile)
{
	(void)profile;
	return ((uint8_t)255);
}

int			farmer_loot(profile_t *profile, req_t *req)
{
	(void)profile;
	(void)req;
	return (0);
}

int			farmer_voir_cb(req_t *req);

int			farmer_find(profile_t *profile)
{
	(void)profile;
	return (0);
}

int			farmer_move(profile_t *profile)
{
	int		r = 0;
	(void)profile;
	return (r);
}

/* the happy farmer */
int 	farmer_fsm(profile_t *profile)
{
	int		r = 0;
	req_t *req = NULL;

	// TODO check if we're still at the look pos
	// else profile->task = FARMER_LOOK;
	if ((req = zap_com_req_alloc(profile->zap)) != NULL)
	{ // Is there a request available ?
		fprintf(stderr, "%s:%d req=%p profile->state=%d\n", __func__, __LINE__, req, profile->state);
		switch (profile->state)
		{
			case (FARMER_LOOK):
				req->zap = profile->zap,
				req->profile = profile,
				req->cb = farmer_voir_cb;
				memcpy(req->buf, commands[CMD_VOIR].name, commands[CMD_VOIR].len);
				req->io_len = commands[CMD_VOIR].len;
				req->cmd_id = CMD_VOIR;
				zap_send_req(profile->zap, req);
				profile->state = FARMER_WAIT;
				break ;
			case (FARMER_FIND):
				fprintf(stderr, "%s:%d FARMER_FIND\n", __func__, __LINE__);
				r = farmer_find(profile);
				/* FIND juste look for food into map and 
				if one found set a rel_target in farmer ctx and switch to MOVE
				if no found and we're still at the LOOK case switch to RANDOM_MOVE
				*/
				if (r == 1) {
					profile->state = FARMER_MOVE;
				}
				if (r == 0 && ((farmer_t*)profile->ctx)->rel_pos == 0) {
					profile->state = FARMER_RANDOM_MOVE;
				}
				break ;
			case (FARMER_RANDOM_MOVE): {
				/* RANDOM_MOVE just move on case to a given random direction 
				(can be front, right or left, not backward)
				the priority of profile decrease at this point
				*/
				profile->state = FARMER_LOOK;
				break;
			}
			case (FARMER_MOVE): {
				fprintf(stderr, "%s:%d FARMER_MOVE\n", __func__, __LINE__);
				r = farmer_move(profile);
				if (r == 1) {
					profile->state = FARMER_LOOT;
				}
				/* MOVE look in the map for some relpos
				if one have food it move to it and when farmer ctx relpos have food it switch to loot
				if there is no food it should move randomly and switch to LOOK, but with a decreasing priority
				to allow another profile to move to its location
				*/
				break ;
			}
			case (FARMER_LOOT): {
				req->zap = profile->zap,
				req->profile = profile,
				fprintf(stderr, "%s:%d FARMER_LOOT\n", __func__, __LINE__);
				r = farmer_loot(profile, req);
				if (r == 1) {
					r = 0;
					profile->state = FARMER_LOOK;
				}
				break ;
			}
			case (FARMER_WAIT):
				fprintf(stderr, "%s:%d FARMER_WAIT\n", __func__, __LINE__);
			default:
				break ;
		}
	}
	else {
		fprintf(stderr, "%s:%d no req available\n", __func__, __LINE__);
	}
	return (r);
}

int			farmer_voir_cb(req_t *req)
{
	(void)req;
	return (0);
}

/*

int			farmer_voir_cb(req_t *req)
{
	int r = 0;
	case_t *c = NULL;

	if (r == 0)
	{
		memcpy(&((farmer_t*)req->profile->ctx)->snap, req->zap->map, sizeof(map_t));
		((farmer_t*)req->profile->ctx)->rel_pos = 0;
		((farmer_t*)req->profile->ctx)->rel_tgt = 0;
		bool found= false;
		for (uint32_t i = req->zap->vision->rel_pos ; i < req->zap->vision->rel_size ; i++) {
			// TODO this function shall search in the snap map instead
			r = zap_map_get_rel(req->zap->map, i, &c);
			if (r == 0)
			{
				if (c->content[R_NOURRITURE] > 0) {
					found = true;
					fprintf(stderr, "%s:%d found one food at relpos=%d from snapnshot x=%d y=%d\n", __func__, __LINE__, i,
					((farmer_t*)req->profile->ctx)->snap.pos_x,
					((farmer_t*)req->profile->ctx)->snap.pos_y);
				}
				((farmer_t*)req->profile->ctx)->rel_tgt = i;
				break ;
			}
		}
		if (!found)
		{
			// FARMER_MOVERANDOM ?
			r = 1;
		}
		else
		{
			if (req->zap->vision->rel_pos == ((farmer_t*)req->profile->ctx)->rel_tgt)
			{
				req->profile->state = FARMER_MOVE;
			}
		}
		fprintf(stderr, "%s:%d FARMER SWITCH STATE TO LOOT\n", __func__, __LINE__);
	}
	return (r);
}

*/

int	farmer_init_cb(profile_t *profile)
{
	farmer_t *farmer_ctx = malloc(sizeof(farmer_t));
	profile->ctx = farmer_ctx;
	return (0);
}
int	farmer_init(zap_t *zap)
{
	profile_t *fprofile = malloc(sizeof(profile_t));
	fprofile->fsm_cb = farmer_fsm;
	fprofile->prio_cb = farmer_prio;
	fprofile->init_cb = farmer_init_cb;
	fprofile->status = 0;
	fprofile->state = FARMER_LOOK;
	fprofile->zap = zap;
	INIT_LIST_HEAD(&fprofile->lst);
	fprofile->ctx = NULL; //(void*)&farmer_ctx
	zap_register_profile(zap, fprofile);

int	farmer_init_cb(profile_t *profile);

	return (0);
}
