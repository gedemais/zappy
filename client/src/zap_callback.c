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
	return (list_empty(&zap->team.broadcast_free) ? NULL : list_first_entry(&zap->team.broadcast_free, broadcast_t ,lst));
}

int	zap_message_cb(zap_t *zap)
{
	broadcast_t *brd = get_broadcast(zap);
	if (brd) {
		brd->direction = atoi(&zap->com.buf_rx[7]);
		int i = -1;
		while (++i < ZAP_RX_BUFSIZE) {
			if (zap->com.buf_rx[i] == ',') {
				i++;
				break;
			}
		}
		fprintf(stderr, "%s: [ID=%d] adding broadcast message to history direction=%d len=%d\n", __func__,
		zap->player.id, brd->direction, zap->com.buf_rx_len - i);
		memcpy(brd->msg, &zap->com.buf_rx[i], zap->com.buf_rx_len - (i+1));
		brd->msg_len = zap->com.buf_rx_len - (i+1);
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

	memset(zap->vision.c, 0, sizeof(zap->vision.c));
	if (zap_parse_voir(zap) == 0)
	{
	
		for (uint32_t i = 0 ; i < req->zap->vision.size ; i++) {
			fprintf(stderr, "%s: c[%d]={", __func__, i);;
			for (int j = 0 ; j < R_MAX ; j++) {
				fprintf(stderr, "%d ", (((uint8_t*)&req->zap->vision.c[i])[j]));;
			}
			fprintf(stderr, "}\n");;
		}
		req->zap->vision.requested = false;
		req->zap->vision.in = true;
		memcpy(&req->zap->vision.coord, &zap->coord, sizeof(coord_t));
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
		zap_abs_avance(req->zap);
		r = zap_vision_avance(zap);
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
