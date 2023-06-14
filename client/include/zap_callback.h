
#ifndef ZAP_CALLBACK_H
# define ZAP_CALLBACK_H

typedef struct req_s req_t;

int	zap_message_cb(zap_t *zap);
int	zap_mort_cb(zap_t *zap);
int	zap_niveau_cb(zap_t *zap);
int	zap_deplacement_cb(zap_t *zap);

int		zap_connect_nbr_cb(req_t *req);
int		zap_inventaire_cb(req_t *req);
int		zap_voir_cb(req_t *req);
int		zap_move_cb(req_t *req);
int		zap_avance_cb(req_t *req);
int		zap_droite_cb(req_t *req);
int		zap_gauche_cb(req_t *req);
int		zap_prend_cb(req_t *req);
int		zap_pose_cb(req_t *req);
int		zap_expulse_cb(req_t *req);
int		zap_broadcast_cb(req_t *req);
int		zap_incantation_cb(req_t *req);
int		zap_fork_cb(req_t *req);

#endif
