#include "zap.h"
#include "zap_map.h"
#include "zap_callback.h"

// execute zap_cb for this given cmd id
// func are in zap_callback.c
// def of command array are in zap_def.h
// this cb is used to update all zap context :
// team, map, stuff
// note that cb mort, deplacement, message do it too
// so, zap ctx is updated before profile-specific cb

static broadcast_t *get_broadcast(zap_t *zap)
{
	broadcast_t *bc = list_last_entry(&zap->team.broadcast, broadcast_t, lst);
	memset(bc->msg, 0, 255);
	bc->msg_len = 0;
	list_del(&bc->lst);
	list_add(&bc->lst, &zap->team.broadcast);
	fprintf(stderr, "%s: [ID=%d] alloc broadcast=%p\n", __func__, zap->player.id, bc);
	// list_add_tail(&cs->lst, &zap->vision.cs);
	return (bc);
}

int	zap_message_cb(zap_t *zap)
{
	broadcast_t *brd = get_broadcast(zap);
	if (brd) {
		list_del(&brd->lst);
		brd->dir = atoi(&zap->com.buf_rx[7]);
		int i = -1;
		while (++i < ZAP_RX_BUFSIZE) {
			if (zap->com.buf_rx[i] == ',') {
				i++;
				break;
			}
		}
		if (i == ZAP_RX_BUFSIZE) {
			fprintf(stderr, "[ERROR] [ID=%d]\n", zap->player.id);
			return (-1);
		}
		memcpy(brd->msg, &zap->com.buf_rx[i], zap->com.buf_rx_len - (i+1));
		if (!memcmp(brd->msg, "player_id", strlen("player_id"))) {
			brd->id = atoi(&zap->com.buf_rx[i + strlen("player_id") + 1]);
			i += strlen("player_id") + 1;
		}
		else {
			brd->id = -1;
		}
		brd->msg_len = zap->com.buf_rx_len - (i+1);
		fprintf(stderr, "%s: [ID=%d] adding broadcast message to history direction=%d len=%d src_id=%d\n", __func__,
			zap->player.id, brd->dir, brd->msg_len, brd->id);
		list_add_tail(&brd->lst, &zap->team.broadcast);
	}
	else {
		fprintf(stderr, "broadcast received but no more memory\n");
	}
	return (0);
}
int	zap_mort_cb(zap_t *zap)
{
	fprintf(stderr, "You lose! MORT [ID=%d] pos_x=%d pos_y=%d dir=%d\n", zap->player.id, zap->coord.abs_pos.pos_x, zap->coord.abs_pos.pos_x, zap->coord.abs_direction);
	zap->player.alive = false;
	return (0);
}
int	zap_niveau_cb(zap_t *zap)
{
	(void)zap;
	return (0);
}
int	zap_deplacement_cb(zap_t *zap)
{
	// TODO DIRECTION
	(void)zap;
	return (0);
}

int		zap_connect_nbr_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	req->zap->player.id = atoi(req->zap->com.buf_rx);
	req->zap->time += 0;
	return (r);
}
int		zap_inventaire_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (zap_parse_inventaire(zap) == 0)
	{
		fprintf(stderr, "%s:%d parse inventaire = {", __func__, __LINE__);
		for (int i = 0 ; i < R_MAX ; i++) {
			fprintf(stderr, "%d ", zap->player.stuff.content[i]);
		}
		fprintf(stderr, "}\n");
		if (r == 0) {
			req->zap->time = 1;
		}
	}
	else
	{
		fprintf(stderr, "%s:%d [ERROR]  [ID=%d] req=%p parse inventaire: buf={%s}drifting packet?\n", __func__, __LINE__,
				zap->player.id, req, zap->com.buf_rx);
		r = -1;
	}
	return (r);
}

int		zap_voir_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	// memset(zap->vision.c, 0, sizeof(zap->vision.c));
	if (zap_parse_voir(zap) == 0)
	{
		case_t *cs = NULL;
		int i = 0;
		list_for_each_entry(cs, &req->zap->vision.cs, lst) {
			fprintf(stderr, "%s: [ID=%d] c=%p cur_pos={%d %d} case_pos={%d %d} c[%d]= {", __func__, 
					req->zap->player.id,
					cs,
					req->zap->coord.__x,
					req->zap->coord.__y,
					cs->coord.__x, cs->coord.__y, i++);;
			for (int j = 0 ; j < R_MAX ; j++) {
				fprintf(stderr, "%d ", (((uint8_t*)cs->content)[j]));;
			}
			fprintf(stderr, "}\n");;
			if (i > 3)
				break ;
		}
		if (req->zap->vision.enabled) {
			req->zap->vision.requested = false;
		}
#ifdef VERBOSE
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		fprintf(stderr, "%s:%d r=%d refresh vision map\n", __func__, __LINE__, r);
#endif
		req->zap->time += 7; // 7 unit time
	}
	else {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p parse voir: buf={%s}drifting packet?\n", __func__, __LINE__,
				zap->player.id, req, zap->com.buf_rx);
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_avance_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
		fprintf(stderr, "%s: [ERROR] [ID=%d] req=%p failed buf={%s}\n", __func__,
				zap->player.id, req, zap->com.buf_rx);
	}
	if (r == 0) {
		zap_abs_avance(req->zap); // moving zap->coord
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================ // response: ok/ko
int		zap_droite_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
		fprintf(stderr, "%s: [ERROR] [ID=%d] req=%p failed\n", __func__,
				zap->player.id, req);
	}
	if (r == 0) {
		zap_abs_droite(req->zap);
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_gauche_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
		fprintf(stderr, "%s: [ERROR] [ID=%d] req=%p failed\n", __func__, zap->player.id, req);
	}
	if (r == 0) {
		zap_abs_gauche(req->zap);
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_prend_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
		fprintf(stderr, "%s: [ERROR] [ID=%d] req=%p prend failed\n", __func__, zap->player.id, req);
	}
	else {
		//for (int i = 0 ; i < R_MAX ; i++) {
		//	if (!memcmp(&req->buf[6], ressources[i].name, ressources[i].len)) {
		//		req->zap->player.stuff.content[i]++;
		//	}
		//}
		fprintf(stderr, "%s:%d PREND stuff={", __func__, __LINE__);
		for (int i = 0 ; i < R_MAX ; i++) {
			fprintf(stderr, "%d ", req->zap->player.stuff.content[i]);
		}
		fprintf(stderr, "}\n");
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_pose_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p\n", __func__, __LINE__, zap->player.id, req);
		r = -1;
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_expulse_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p\n", __func__, __LINE__, zap->player.id, req);
		r = -1;
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_broadcast_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p\n", __func__, __LINE__, zap->player.id, req);
		r = -1;
	}
	req->zap->time += 7; // 7 unit time
	return (r);
}

// ============================================================================

// response: elevation en cours/ko
int		zap_incantation_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "elevation en cours", strlen("ok"))) {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p\n", __func__, __LINE__, zap->player.id, req);
		r = -1;
	}
	// req->zap->time += 7; // 7 unit time TODO
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_fork_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		fprintf(stderr, "%s:%d [ERROR] [ID=%d] req=%p\n", __func__, __LINE__, zap->player.id, req);
		r = -1;
	}
	req->zap->time += 42; // 42 unit time
	return (r);
}

// ============================================================================
