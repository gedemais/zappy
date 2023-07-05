#ifndef GRAPHICAL_H
# define GRAPHICAL_H

enum	e_graphical_commands
{
	GCMD_MAP_SIZE,
	GCMD_BLOCK_CONTENT,
	GCMD_MAP_CONTENT,
	GCMD_TEAMS_NAME,
	GCMD_PLAYER_POSITION,
	GCMD_PLAYER_LEVEL,
	GCMD_PLAYER_INVENTORY,
	GCMD_SGT,
	GCMD_SST,
	GCMD_MAX
};

static const char *gcmd_names[GCMD_MAX] = {

	[GCMD_MAP_SIZE] = "msz",
	[GCMD_BLOCK_CONTENT] = "bct",
	[GCMD_MAP_CONTENT] = "mct",
	[GCMD_TEAMS_NAME] = "tna",
	[GCMD_PLAYER_POSITION] = "ppo",
	[GCMD_PLAYER_LEVEL] = "plv",
	[GCMD_PLAYER_INVENTORY] = "pin",
	[GCMD_SGT] = "sgt",
	[GCMD_SST] = "sst"
};



#endif
