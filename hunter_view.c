////////////////////////////////////////////////////////////////////////
// COMP2521 19t0 ... the Fury of Dracula
// hunter_view.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>

#include "game.h"
#include "game_view.h"
#include "hunter_view.h"
// #include "map.h" ... if you decide to use the Map ADT

typedef struct hunt_Char
{
	int health;
	location_t location;
} hunt_Char;
typedef hunt_Char *Hunt_Char;
typedef struct hunter_view {
	round_t turn; // current turn, starting at 0
	int score;
	//use enums to determine current player
	int currPlayer;
	Hunt_Char ch[5];
} hunter_view;
hunter_view *hv_new (char *past_plays, player_message messages[])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	hunter_view *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate HunterView");

	return new;
}

void hv_drop (hunter_view *hv)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free (hv);
}

round_t hv_get_round (hunter_view *hv)
{
	return hv.turn;
}

enum player hv_get_player (hunter_view *hv)
{
	return hv.currPlayer;
}

int hv_get_score (hunter_view *hv)
{
	return hv.score;
}

int hv_get_health (hunter_view *hv, enum player player)
{
	return hv->ch[player].health;
}

location_t hv_get_location (hunter_view *hv, enum player player)
{
	return hv->ch[player].location;
}

void hv_get_trail (
	hunter_view *hv, enum player player,
	location_t trail[TRAIL_SIZE])
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

location_t *hv_get_dests (
	hunter_view *hv, size_t *n_locations,
	bool road, bool rail, bool sea)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*n_locations = 0;
	return NULL;
}

location_t *hv_get_dests_player (
	hunter_view *hv, size_t *n_locations, enum player player,
	bool road, bool rail, bool sea)
{
	/// @todo REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*n_locations = 0;
	return NULL;
}
