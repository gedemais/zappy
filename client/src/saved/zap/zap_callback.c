#include "zap.h"
#include "zap_map.h"
#include "zap_callback.h"

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

	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	// r = zap_parse_voir(&req->zap->vision->map, req->zap->com);
	// TODO
	fprintf(stderr, "%s:%d r=%d refresh vision map\n", __func__, __LINE__, r);
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
	}
	//update_vision(req->zap->vision);
	//update_map(req->zap->map);
	return (r);
}

// ============================================================================

// response: ok/ko
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
	}
	fprintf(stderr, "%s:%d\n", __func__, __LINE__);
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
