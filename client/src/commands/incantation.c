#include "zappy_client.h"


static int	requirements_ressources(int *inventaire, int *requirements)
{
	int	r = 1;

	// i = 1 car on skip la ressource nourriture et
	// R_MAX - 1 pour ne pas prendre en compte le ttl
	for (int i = 1; i < R_MAX - 1; i++) {
		if (inventaire[i] < requirements[i]) {
			r = 0;
			break ;
		}
	}
	return (r);
}

static int	requirements_case_players(int *inventaire, int *requirements)
{
	(void)inventaire;
	(void)requirements;

	int	r = 0;

	return (r);
}

/*
	choice 0 : ressources in inventaire (player or case) requirement
	choice 1 : players on case requirement
	-
	return 0
	return 1 if requirements are OK
*/
int		incantation_requirements(t_inventaire inventaire, uint8_t lvl, uint8_t choice)
{
	int	r = 0;
	int	*ptr_inventaire = (int *)&inventaire;
	int	*ptr_requirement = (int *)&requirements[lvl];

	if (choice == 0) {
		r = requirements_ressources(ptr_inventaire, ptr_requirement);
	} else if (choice == 1) {
		r = requirements_case_players(ptr_inventaire, ptr_requirement);
	}
	return (r);
}

// response: elevation en cours
// niveau actuel : K
int		zappy_incantation_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}
