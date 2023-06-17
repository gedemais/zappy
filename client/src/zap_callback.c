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

int	zap_message_cb(zap_t *zap)
{
	(void)zap;
	return (0);
}
int	zap_mort_cb(zap_t *zap)
{
	fprintf(stderr, "You lose! pos_x=%d pos_y=%d dir=%d\n", zap->coord.abs_pos.pos_x, zap->coord.abs_pos.pos_x, zap->coord.abs_direction);
	(void)zap;
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

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}
int		zap_inventaire_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

int		zap_voir_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	memset(zap->vision.c, 0, sizeof(zap->vision.c));
	r = zap_parse_voir(zap);
fprintf(stderr, "%s:%s:%d parse voir r=%d len=%d\n", __FILE__, __func__, __LINE__, r ,req->zap->vision.size);
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
	}
	if (r == 0) {
		zap_abs_avance(req->zap);
		r = zap_vision_avance(zap);
	}
	//update_vision(req->zap->vision);
	//update_map(req->zap->map);
	return (r);
}

// ============================================================================ // response: ok/ko
int		zap_droite_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	if (r == 0) {
		zap_abs_droite(req->zap);
	}
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
	}
	if (r == 0) {
		zap_abs_gauche(req->zap);
	}
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
		fprintf(stderr, "[ERROR] prend failed\n");
	}
	else {
		for (int i = 0 ; i < R_MAX ; i++) {
			if (!memcmp(&req->buf[6], ressources[i].name, ressources[i].len)) {
				req->zap->player.stuff.content[i]++;
			}
		}
		fprintf(stderr, "%s:%d PREND stuff={", __func__, __LINE__);
		for (int i = 0 ; i < R_MAX ; i++) {
			fprintf(stderr, "%d ", req->zap->player.stuff.content[i]);
		}
		fprintf(stderr, "}\n");
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_pose_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_expulse_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_broadcast_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: elevation en cours/ko
int		zap_incantation_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "elevation en cours", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zap_fork_cb(req_t *req)
{
	zap_t *zap = req->zap;
	int r = 0;

	if (memcmp(zap->com.buf_rx, "ok", strlen("ok"))) {
		r = -1;
	}
	return (r);
}

// ============================================================================
